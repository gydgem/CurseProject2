#include "Client.h"

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
                    catch (const std::exception &e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                } else if (cmd == "upload") {
                    size_t pos2 = input.find(' ', pos + 1);
                    if (pos2 == std::string::npos) throw std::invalid_argument("Invalid arguments");

                    std::string local = input.substr(pos + 1, pos2 - pos - 1);
                    std::string remote = input.substr(pos2 + 1);
                    client.upload_file(local, remote);
                    std::cout << "File uploaded successfully\n";
                } else if (cmd == "download") {
                    size_t pos2 = input.find(' ', pos + 1);
                    if (pos2 == std::string::npos) throw std::invalid_argument("Invalid arguments");

                    std::string remote = input.substr(pos + 1, pos2 - pos - 1);
                    std::string local = input.substr(pos2 + 1);
                    client.download_file(remote, local);
                    std::cout << "File downloaded successfully\n";
                } else {
                    std::cout << "Unknown command. Type 'help' for help.\n";
                }
            } catch (const std::exception &e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
    } catch (const std::exception &ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    }
    return 0;
}