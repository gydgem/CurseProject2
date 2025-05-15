//
// Created by gydgem on 5/15/2025.
//

#ifndef NETWORK_APP_SERVER_H
#define NETWORK_APP_SERVER_H


#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port);

private:
    void start_accept();

    tcp::acceptor acceptor_;
};


#endif //NETWORK_APP_SERVER_H
