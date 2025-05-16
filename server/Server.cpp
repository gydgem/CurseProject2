//
// Created by gydgem on 5/15/2025.
//

// Server.cpp
#include "Server.h"
#include "Session.h"
#include <iostream>
#include <boost/asio/posix/stream_descriptor.hpp>

Server::Server(boost::asio::io_context &io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          console_input_(io_context, ::dup(STDIN_FILENO)) {
    start_accept();
    start_console_input();
}

void Server::start_console_input() {
    auto handler = [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            std::istream is(&console_buffer_);
            std::string command;
            std::getline(is, command);
            handle_command(command);
            start_console_input();
        }
    };

    async_read_until(console_input_, console_buffer_, '\n', handler);
}

void Server::handle_command(const std::string &command) {
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    if (command == "stop") {
        stop();
    } else if (command == "status") {
        std::cout << "Active sessions: " << sessions_.size()
                  << ", Server status: "
                  << (acceptor_.is_open() ? "RUNNING" : "STOPPED")
                  << "\n";
    }else if (cmd == "help") {
        std::cout << "Available commands:\n"
                  << "  status - Show active sessions\n"
                  << "  stop - Stop the server\n"
                  << "  help - Show this help\n";
    } else {
        std::cout << "Unknown command. Type 'help' for help.\n";
    }
}

void Server::stop() {
    std::cout << "Initiating graceful shutdown...\n";

    boost::system::error_code ec;
    acceptor_.close(ec);
    if (ec) {
        std::cerr << "Acceptor close error: " << ec.message() << "\n";
    }

    std::vector<std::shared_ptr<Session>> sessions_copy;
    {
        sessions_copy.reserve(sessions_.size());
        for (auto& session : sessions_) {
            sessions_copy.push_back(session);
        }
    }

    for (auto& session : sessions_copy) {
        session->close();
    }

    io_context_.stop();
}

void Server::start_accept() {
    acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    try {
                        auto session = Session::create(std::move(socket), *this);
                        session->start();
                    } catch (const std::exception& e) {
                        std::cerr << "Session creation error: " << e.what() << "\n";
                    }
                }
                start_accept();
            });
}

boost::asio::io_context &Server::get_io_context() { return io_context_; }
