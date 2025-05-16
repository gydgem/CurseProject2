//
// Created by gydgem on 5/16/2025.
//

#ifndef NETWORK_APP_CLIENT_H
#define NETWORK_APP_CLIENT_H

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <fstream>

using boost::asio::ip::tcp;

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& host, short port);

    std::string send_command(const std::string& request);

    std::string execute_command(const std::string& command);

    void upload_file(const std::string& local_path, const std::string& remote_filename);

    void download_file(const std::string& remote_filename, const std::string& local_path);

private:
    tcp::socket socket_;
};


#endif //NETWORK_APP_CLIENT_H
