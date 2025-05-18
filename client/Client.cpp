//
// Created by gydgem on 5/16/2025.
//

#include "Client.h"

Client::Client(boost::asio::io_context &io_context, const std::string &host, short port)
        : socket_(io_context) {
    tcp::resolver resolver(io_context);
    boost::asio::connect(socket_, resolver.resolve(host, std::to_string(port)));
}

std::string Client::send_command(const std::string &request) {
    std::string full_request = request + "\nEND_CMD\n"; // Добавляем маркер конца
    boost::asio::write(socket_, boost::asio::buffer(full_request));

    boost::asio::streambuf buffer;
    boost::asio::read_until(socket_, buffer, "\nEND_CMD\n"); // Читаем до маркера

    std::istream is(&buffer);
    std::string response;
    std::getline(is, response, '\0');

    size_t end_pos = response.rfind("\nEND_CMD");
    if (end_pos != std::string::npos) {
        response = response.substr(0, end_pos);
    }

    return response;
}

std::string Client::execute_command(const std::string &command) {
    std::string response = send_command("COMMAND " + command);

    if (response.substr(0, 11) == "CMD_RESULT\n") {
        return response.substr(11); // Без декодирования
    } else if (response.substr(0, 10) == "CMD_ERROR\n") {
        throw std::runtime_error(response.substr(10)); // Без декодирования
    }
    return response;
}

void Client::upload_file(const std::string &local_path, const std::string &remote_filename) {
    std::ifstream file(local_path);
    std::string content((std::istreambuf_iterator<char>(file)), {});
    std::string request = "UPLOAD " + remote_filename + " " + content; // Без base64
    send_command(request);
}

void Client::download_file(const std::string &remote_filename, const std::string &local_path) {
    std::string response = send_command("DOWNLOAD " + remote_filename);
    if (response.substr(0, 9) == "DOWNLOAD ") {
        std::ofstream file(local_path);
        file << response.substr(9);
    } else {
        throw std::runtime_error(response);
    }
}
