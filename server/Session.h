//
// Created by gydgem on 5/15/2025.
//

#ifndef NETWORK_APP_SESSION_H
#define NETWORK_APP_SESSION_H


#include <boost/asio.hpp>
#include <memory>
#include "Workspace.h"
#include "Server.h"

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    friend class Server;
private:
    Session(tcp::socket socket, Server& server); // Делаем конструктор приватным
    std::weak_ptr<Session> self_; // Добавляем weak_ptr на себя

public:
    ~Session();
    void start();
    void close();
    static std::shared_ptr<Session> create(tcp::socket socket, Server& server);

private:
    void do_read();
    void do_write(const std::string& message);
    std::string process_request(const std::string& request);

    Server& server_;
    tcp::socket socket_;
    boost::asio::streambuf buffer_;
    Workspace workspace_;
    std::mutex socket_mutex_;  // Мьютекс для синхронизации доступа к сокету
    bool socket_active_ = true; // Флаг состояния сокета
};


#endif //NETWORK_APP_SESSION_H
