#ifndef NETWORK_APP_COMMANDHANDLER_H
#define NETWORK_APP_COMMANDHANDLER_H


#include <string>

class CommandHandler {
public:
    virtual ~CommandHandler() = default;
    virtual void handleCommand(const std::string& command) = 0;
    virtual void printHelp() const = 0;
};

#endif //NETWORK_APP_COMMANDHANDLER_H
