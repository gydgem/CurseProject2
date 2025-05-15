//
// Created by gydgem on 5/15/2025.
//

#include "Session.h"
#include "RequestHandler.h"
#include <iostream>

Session::Session(tcp::socket socket) : socket_(std::move(socket)) {}

void Session::start() { do_read(); }

void Session::do_read() {
    auto self(shared_from_this());
    boost::asio::async_read_until(
            socket_, buffer_, '\n',
            [this, self](boost::system::error_code ec, size_t length) {
                if (!ec) {
                    std::string message{
                            boost::asio::buffers_begin(buffer_.data()),
                            boost::asio::buffers_begin(buffer_.data()) + length - 1
                    };
                    buffer_.consume(length);

                    std::cout << "Received: " << message << std::endl;
                    std::string response = RequestHandler::process(message);

                    do_write(response + "\n");
                } else {
                    if (ec != boost::asio::error::eof) {
                        std::cerr << "Error: " << ec.message() << std::endl;
                    }
                    socket_.close();
                }
            });
}

void Session::do_write(const std::string& message) {
    auto self(shared_from_this());
    boost::asio::async_write(
            socket_, boost::asio::buffer(message),
            [this, self](boost::system::error_code ec, size_t /*length*/) {
                if (!ec) do_read();
                else socket_.close();
            });
}
