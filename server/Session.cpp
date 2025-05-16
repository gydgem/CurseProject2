//
// Created by gydgem on 5/15/2025.
//

#include "Session.h"
#include "base64.h"
#include <boost/process.hpp>
#include <sstream>
#include <iostream>

using namespace boost::asio;

Session::Session(tcp::socket socket, Server &server)
        : socket_(std::move(socket)),
          server_(server) {
}

Session::~Session() {
    auto ptr = self_.lock();
    if (ptr) {
        server_.sessions_.erase(ptr);
    }
}

void Session::start() {
    server_.sessions_.insert(self_.lock());
    do_read();
}

void Session::do_read() {
    auto self(shared_from_this());
    async_read_until(
            socket_,
            buffer_,
            "\nEND_CMD\n",
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::string message{
                            buffers_begin(buffer_.data()),
                            buffers_begin(buffer_.data()) + length - 9
                    };
                    buffer_.consume(length);

                    std::string response = process_request(message);

                    do_write(response + "\n");
                } else if (ec != boost::asio::error::eof) {
                    std::cerr << "Read error: " << ec.message() << std::endl;
                    socket_.close();
                }
            });
}

std::string Session::process_request(const std::string &request) {
    std::istringstream iss(request);
    std::string command;
    iss >> command;
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "COMMAND") {
        std::string cmd;
        std::getline(iss >> std::ws, cmd);
        try {
            return "CMD_RESULT\n" + workspace_.execute_command(cmd);
        } catch (const std::exception &e) {
            return "CMD_ERROR\n" + std::string(e.what());
        }
    } else if (command == "UPLOAD") {
        std::string filename, content;
        iss >> filename;
        std::getline(iss >> std::ws, content);
        try {
            workspace_.upload_file(filename, content);
            return "UPLOAD_OK";
        } catch (...) {
            return "UPLOAD_ERROR";
        }
    } else if (command == "DOWNLOAD") {
        std::string filename;
        iss >> filename;
        try {
            std::string content = workspace_.download_file(filename);
            return "DOWNLOAD " + content; // Без base64
        } catch (...) {
            return "DOWNLOAD_ERROR";
        }
    }
    return "ERROR: Unknown command";
}

void Session::do_write(const std::string &message) {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    if (!socket_active_) return;

    std::string framed = message + "\nEND_CMD\n";
    auto self(shared_from_this());
    async_write(
            socket_,
            buffer(framed),
            [this, self](boost::system::error_code ec, size_t /*length*/) {
                std::lock_guard<std::mutex> lock(socket_mutex_);
                if (ec) {
                    socket_active_ = false;
                    socket_.close();
                    return;
                }
                do_read();
            });
}


void Session::close() {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    if (socket_active_) {
        socket_active_ = false;
        boost::system::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_both, ec);
        socket_.close(ec);
    }
}

std::shared_ptr<Session> Session::create(tcp::socket socket, Server &server) {
    auto ptr = std::shared_ptr<Session>(new Session(std::move(socket), server));
    ptr->self_ = ptr;
    return ptr;
}

