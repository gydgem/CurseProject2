// Server.h
#ifndef NETWORK_APP_SERVER_H
#define NETWORK_APP_SERVER_H

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>

using boost::asio::ip::tcp;

class Server {
public:
    friend class Session;

    Server(boost::asio::io_context& io_context, short port);
    void stop();
    boost::asio::io_context& get_io_context();

private:
    void start_accept();
    void start_console_input();
    void handle_command(const std::string& command);

    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    std::unordered_set<std::shared_ptr<class Session>> sessions_;


    boost::asio::posix::stream_descriptor console_input_;
    boost::asio::streambuf console_buffer_;
};

#endif //NETWORK_APP_SERVER_H