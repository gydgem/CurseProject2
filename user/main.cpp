#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <fstream>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& host, short port)
            : socket_(io_context) {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket_, resolver.resolve(host, std::to_string(port)));
    }

    std::string send_command(const std::string& request) {
        std::string full_request = request + "\nEND_CMD\n"; // Добавляем маркер конца
        boost::asio::write(socket_, boost::asio::buffer(full_request));

        boost::asio::streambuf buffer;
        boost::asio::read_until(socket_, buffer, "\nEND_CMD\n"); // Читаем до маркера

        std::istream is(&buffer);
        std::string response;
        std::getline(is, response, '\0');

        // Обрезаем маркер конца
        size_t end_pos = response.rfind("\nEND_CMD");
        if (end_pos != std::string::npos) {
            response = response.substr(0, end_pos);
        }

        return response;
    }

    std::string execute_command(const std::string& command) {
        std::string response = send_command("COMMAND " + command);

        if (response.substr(0, 11) == "CMD_RESULT\n") {
            return response.substr(11); // Без декодирования
        } else if (response.substr(0, 10) == "CMD_ERROR\n") {
            throw std::runtime_error(response.substr(10)); // Без декодирования
        }
        return response;
    }

    void upload_file(const std::string& local_path, const std::string& remote_filename) {
        std::ifstream file(local_path);
        std::string content((std::istreambuf_iterator<char>(file)), {});
        std::string request = "UPLOAD " + remote_filename + " " + content; // Без base64
        send_command(request);
    }

    void download_file(const std::string& remote_filename, const std::string& local_path) {
        std::string response = send_command("DOWNLOAD " + remote_filename);
        if (response.substr(0, 9) == "DOWNLOAD ") {
            std::ofstream file(local_path);
            file << response.substr(9); // Без декодирования
        } else {
            throw std::runtime_error(response);
        }
    }


private:
    tcp::socket socket_;
};

void print_help() {
    std::cout << "Available commands:\n"
              << "  command <shell command>  - Execute shell command\n"
              << "  upload <local> <remote> - Upload file to server\n"
              << "  download <remote> <local> - Download file from server\n"
              << "  exit - Quit client\n";
}

int main() {
    try {
        boost::asio::io_context io_context;
        Client client(io_context, "localhost", 1234);
        std::cout << "Connected to server. Type 'help' for commands list.\n";

        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.empty()) continue;

            try {
                if (input == "exit") break;

                if (input == "help") {
                    print_help();
                    continue;
                }

                size_t pos = input.find(' ');
                std::string cmd = (pos == std::string::npos) ? input : input.substr(0, pos);

                if (cmd == "command" && pos != std::string::npos) {
                    std::string command = input.substr(pos + 1);
                    try {
                        std::string result = client.execute_command(command);
                        std::cout << "Command result:\n" << result << std::endl;
                    }
                    catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                }
                else if (cmd == "upload") {
                    size_t pos2 = input.find(' ', pos + 1);
                    if (pos2 == std::string::npos) throw std::invalid_argument("Invalid arguments");

                    std::string local = input.substr(pos + 1, pos2 - pos - 1);
                    std::string remote = input.substr(pos2 + 1);
                    client.upload_file(local, remote);
                    std::cout << "File uploaded successfully\n";
                }
                else if (cmd == "download") {
                    size_t pos2 = input.find(' ', pos + 1);
                    if (pos2 == std::string::npos) throw std::invalid_argument("Invalid arguments");

                    std::string remote = input.substr(pos + 1, pos2 - pos - 1);
                    std::string local = input.substr(pos2 + 1);
                    client.download_file(remote, local);
                    std::cout << "File downloaded successfully\n";
                }
                else {
                    std::cout << "Unknown command. Type 'help' for help.\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    }
    return 0;
}