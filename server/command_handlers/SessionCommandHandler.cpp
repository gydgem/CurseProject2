#include "SessionCommandHandler.h"
#include <boost/process.hpp>

SessionCommandHandler::SessionCommandHandler(SessionContext context)
        : context_(context) {}

std::string SessionCommandHandler::handleRequest(const std::string &request) {
    std::istringstream iss(request);
    std::string command;
    iss >> command;
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);

    if (command == "CREATE_ZONE") return handleCreateZone(iss);
    if (command == "ENTER_ZONE") return handleEnterZone(iss);
    if (command == "LEAVE_ZONE") return handleLeaveZone();
    if (command == "LIST_ZONES") return handleListZones();
    if (command == "COMMAND") return handleCommand(iss);
    if (command == "UPLOAD") return handleUpload(iss);
    if (command == "DOWNLOAD") return handleDownload(iss);

    return "ERROR: Unknown command";
}

std::string SessionCommandHandler::handleCreateZone(std::istringstream &iss) {
    if (!context_.current_zone.empty())
        return "ERROR: Already in zone";

    std::string name;
    if (!(iss >> name))
        return "ERROR: Missing zone name";

    if (!context_.zone_manager.create_zone(name))
        return "ERROR: Zone exists";

    return "ZONE_CREATED " + name;
}

std::string SessionCommandHandler::handleEnterZone(std::istringstream &iss) {
    if (!context_.current_zone.empty())
        return "ERROR: Already in zone";

    std::string name;
    if (!(iss >> name))
        return "ERROR: Missing zone name";

    if (!context_.zone_manager.enter_zone(name))
        return "ERROR: Zone unavailable";

    context_.workspace = Workspace(context_.zone_manager.get_zone_path(name));
    context_.current_zone = name;
    return "ENTERED_ZONE " + name;
}

std::string SessionCommandHandler::handleLeaveZone() {
    if (context_.current_zone.empty())
        return "ERROR: Not in zone";

    context_.zone_manager.leave_zone(context_.current_zone);
    context_.current_zone.clear();
    context_.workspace = Workspace();
    return "LEFT_ZONE";
}

std::string SessionCommandHandler::handleListZones() {
    auto zones = context_.zone_manager.list_zones();
    std::string response = "ZONES:\n";
    for (const auto &z: zones) {
        response += z + "\n";
    }
    return response;
}

std::string SessionCommandHandler::handleCommand(std::istringstream &iss) {
    if (context_.current_zone.empty())
        return "ERROR: No zone selected";

    std::string cmd;
    std::getline(iss >> std::ws, cmd);

    try {
        return "CMD_RESULT\n" + context_.workspace.execute_command(cmd);
    } catch (const std::exception &e) {
        return "CMD_ERROR\n" + std::string(e.what());
    }
}

std::string SessionCommandHandler::handleUpload(std::istringstream &iss) {
    if (context_.current_zone.empty())
        return "ERROR: No zone selected";

    auto [filename, content] = splitTwoArgs(iss);

    try {
        context_.workspace.upload_file(filename, content);
        return "UPLOAD_OK";
    } catch (...) {
        return "UPLOAD_ERROR";
    }
}

std::string SessionCommandHandler::handleDownload(std::istringstream &iss) {
    if (context_.current_zone.empty())
        return "ERROR: No zone selected";

    std::string filename;
    if (!(iss >> filename))
        return "ERROR: Missing filename";

    try {
        std::string content = context_.workspace.download_file(filename);
        return "DOWNLOAD " + content;
    } catch (...) {
        return "DOWNLOAD_ERROR";
    }
}

std::pair<std::string, std::string> SessionCommandHandler::splitTwoArgs(std::istringstream &iss) {
    std::string first, second;
    if (!(iss >> first))
        throw std::invalid_argument("Missing first argument");

    std::getline(iss >> std::ws, second);
    if (second.empty())
        throw std::invalid_argument("Missing second argument");

    return {first, second};
}

std::string SessionCommandHandler::extractContent(std::istringstream &iss) {
    std::string content;
    std::getline(iss >> std::ws, content);
    return content;
}