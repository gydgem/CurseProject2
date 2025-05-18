#ifndef SESSION_COMMAND_HANDLER_H
#define SESSION_COMMAND_HANDLER_H

#include <string>
#include <sstream>
#include "../ZoneManager.h"
#include "../Workspace.h"

class SessionCommandHandler {
public:
    struct SessionContext {
        std::string &current_zone;
        Workspace &workspace;
        ZoneManager &zone_manager;
    };

    explicit SessionCommandHandler(SessionContext context);

    std::string handleRequest(const std::string &request);

private:
    SessionContext context_;

    std::string handleCreateZone(std::istringstream &iss);

    std::string handleEnterZone(std::istringstream &iss);

    std::string handleLeaveZone();

    std::string handleListZones();

    std::string handleCommand(std::istringstream &iss);

    std::string handleUpload(std::istringstream &iss);

    std::string handleDownload(std::istringstream &iss);

    std::pair<std::string, std::string> splitTwoArgs(std::istringstream &iss);

    std::string extractContent(std::istringstream &iss);
};

#endif // SESSION_COMMAND_HANDLER_H