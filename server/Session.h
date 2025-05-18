//
// Created by gydgem on 5/15/2025.
//

#ifndef NETWORK_APP_SESSION_H
#define NETWORK_APP_SESSION_H


#include <memory>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include "Workspace.h"
//#include "Server.h"
#include "command_handlers/SessionCommandHandler.h"


using boost::asio::ip::tcp;
class Server;

class Session : public std::enable_shared_from_this<Session> {
public:
    friend class Server;
private:
    Session(tcp::socket socket, Server& server);
    std::weak_ptr<Session> self_;

public:
    ~Session();
    void start();
    void close();
    static std::shared_ptr<Session> create(tcp::socket socket, Server& server);

private:
    void do_read();
    void do_write(const std::string& message);
    std::string process_request(const std::string& request);
    SessionCommandHandler createCommandHandler();

    Server& server_;
    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Workspace workspace_;
    std::mutex socket_mutex_;
    bool socket_active_ = true;
    std::string current_zone_;
};


#endif //NETWORK_APP_SESSION_H
