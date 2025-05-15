//
// Created by gydgem on 5/15/2025.
//

#ifndef NETWORK_APP_SESSION_H
#define NETWORK_APP_SESSION_H


#include <boost/asio.hpp>
#include <memory>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket);
    void start();

private:
    void do_read();
    void do_write(const std::string& message);

    tcp::socket socket_;
    boost::asio::streambuf buffer_;
};


#endif //NETWORK_APP_SESSION_H
