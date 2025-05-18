#ifndef NETWORK_APP_USERCOMMANDHANDLER_H
#define NETWORK_APP_USERCOMMANDHANDLER_H

#include "Client.h"
#include <string>
#include <iostream>

class UserCommandHandler {
public:
    explicit UserCommandHandler(Client &client);

    void handleCommand(const std::string &input);

    static void printHelp();

private:
    Client &client_;

    void handleCreateZone(const std::string &args);

    void handleEnterZone(const std::string &args);

    void handleLeaveZone();

    void handleListZones();

    void handleShellCommand(const std::string &args);

    void handleUpload(const std::string &args);

    void handleDownload(const std::string &args);

    static std::pair<std::string, std::string> splitTwoArgs(const std::string &args);
};

#endif //NETWORK_APP_USERCOMMANDHANDLER_H
