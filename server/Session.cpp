#include "Session.h"
#include "Server.h"

using namespace boost::asio;

Session::Session(tcp::socket socket, Server &server)
        : socket_(std::move(socket)),
          server_(server) {
}

Session::~Session() {
    close();
    auto ptr = self_.lock();
    if (ptr) {
        std::lock_guard<std::mutex> lock(server_.sessions_mutex_);
        server_.sessions_.erase(ptr);
    }
}

void Session::start() {
    {
        std::lock_guard<std::mutex> lock(server_.sessions_mutex_);
        server_.sessions_.insert(shared_from_this());
    }
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
                } else {
                    if (ec != boost::asio::error::operation_aborted) {
                        close();
                        {
                            std::lock_guard<std::mutex> lock(server_.sessions_mutex_);
                            server_.sessions_.erase(self);
                        }
                    }
                }
            });
}

std::string Session::process_request(const std::string &request) {
    return createCommandHandler().handleRequest(request);
}

void Session::do_write(const std::string &message) {
    std::lock_guard<std::mutex> lock(socket_mutex_);
    if (!socket_active_) return;

    // Добавить создание framed сообщения
    std::string framed = message + "\nEND_CMD\n";

    auto self(shared_from_this());
    async_write(
            socket_,
            buffer(framed),
            [this, self](boost::system::error_code ec, size_t /*length*/) {
                std::lock_guard<std::mutex> lock(socket_mutex_);
                if (ec) {
                    close();
                    {
                        std::lock_guard<std::mutex> lock(server_.sessions_mutex_);
                        server_.sessions_.erase(self);
                    }
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

        if (!current_zone_.empty()) {
            server_.zone_manager_.leave_zone(current_zone_);
            current_zone_.clear();
        }
    }
}

std::shared_ptr<Session> Session::create(tcp::socket socket, Server &server) {
    auto ptr = std::shared_ptr<Session>(new Session(std::move(socket), server));
    ptr->self_ = ptr;
    return ptr;
}

SessionCommandHandler Session::createCommandHandler() {
    return SessionCommandHandler{
            {current_zone_, workspace_, server_.zone_manager_}
    };
}


