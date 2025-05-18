//
// Created by gydgem on 5/18/2025.
//

#ifndef NETWORK_APP_SERVERCOMMANDHHANDLER_H
#define NETWORK_APP_SERVERCOMMANDHHANDLER_H

#include "CommandHandler.h"
#include <iostream>
#include <thread>
#include <mutex>


class Server;

class ServerCommandHandler : public CommandHandler {
public:
    explicit ServerCommandHandler(Server& server);

    void handleCommand(const std::string& command) override;
    void printHelp() const override;

private:
    Server& server_;

    void handleStatus();
    void handleStop();
};

#endif //NETWORK_APP_SERVERCOMMANDHHANDLER_H
