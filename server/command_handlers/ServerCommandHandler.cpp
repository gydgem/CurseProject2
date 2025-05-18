// ServerCommandHandler.cpp
#include "ServerCommandHandler.h"
#include "../Server.h"

ServerCommandHandler::ServerCommandHandler(Server& server)
        : server_(server) {}

void ServerCommandHandler::handleCommand(const std::string& command) {
    if (command == "status") {
        handleStatus();
    } else if (command == "stop") {
        handleStop();
    } else {
        std::cout << "Unknown server command. Type 'help' for help.\n";
    }
}

void ServerCommandHandler::printHelp() const {
    std::cout << "Available server commands:\n"
              << "  status - Show active sessions\n"
              << "  stop - Stop the server\n"
              << "  help - Show this help\n";
}

void ServerCommandHandler::handleStatus() {
    std::lock_guard<std::mutex> lock(server_.sessions_mutex_);
    std::cout << "Active sessions: " << server_.sessions_.size()
              << ", Server status: "
              << (server_.acceptor_.is_open() ? "RUNNING" : "STOPPED")
              << "\n";
}

void ServerCommandHandler::handleStop() {
    server_.stop();
}