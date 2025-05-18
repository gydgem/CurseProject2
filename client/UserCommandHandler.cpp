#include "UserCommandHandler.h"

UserCommandHandler::UserCommandHandler(Client& client) : client_(client) {}

void UserCommandHandler::handleCommand(const std::string& input) {
    size_t pos = input.find(' ');
    std::string cmd = (pos == std::string::npos) ? input : input.substr(0, pos);
    std::string args = (pos == std::string::npos) ? "" : input.substr(pos + 1);

    try {
        if (cmd == "create_zone") {
            handleCreateZone(args);
        } else if (cmd == "enter_zone") {
            handleEnterZone(args);
        } else if (cmd == "leave_zone") {
            handleLeaveZone();
        } else if (cmd == "list_zones") {
            handleListZones();
        } else if (cmd == "command") {
            handleShellCommand(args);
        } else if (cmd == "upload") {
            handleUpload(args);
        } else if (cmd == "download") {
            handleDownload(args);
        } else {
            std::cout << "Unknown command. Type 'help' for help.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }
}

void UserCommandHandler::printHelp() {
    std::cout << "Available commands:\n"
              << "  create_zone <name> - Create new workspace\n"
              << "  enter_zone <name> - Enter existing workspace\n"
              << "  leave_zone - Leave current workspace\n"
              << "  list_zones - List available workspaces\n"
              << "  command <shell command>  - Execute shell command\n"
              << "  upload <local> <remote> - Upload file to server\n"
              << "  download <remote> <local> - Download file from server\n"
              << "  exit - Quit client\n";
}

void UserCommandHandler::handleCreateZone(const std::string& args) {
    if (args.empty()) throw std::invalid_argument("Missing zone name");
    std::cout << client_.send_command("CREATE_ZONE " + args) << "\n";
}

void UserCommandHandler::handleEnterZone(const std::string& args) {
    if (args.empty()) throw std::invalid_argument("Missing zone name");
    std::cout << client_.send_command("ENTER_ZONE " + args) << "\n";
}

void UserCommandHandler::handleLeaveZone() {
    std::cout << client_.send_command("LEAVE_ZONE") << "\n";
}

void UserCommandHandler::handleListZones() {
    std::cout << client_.send_command("LIST_ZONES") << "\n";
}

void UserCommandHandler::handleShellCommand(const std::string& args) {
    if (args.empty()) throw std::invalid_argument("Missing command");
    std::string result = client_.execute_command(args);
    std::cout << "Command result:\n" << result << std::endl;
}

void UserCommandHandler::handleUpload(const std::string& args) {
    auto [local, remote] = splitTwoArgs(args);
    client_.upload_file(local, remote);
    std::cout << "File uploaded successfully\n";
}

void UserCommandHandler::handleDownload(const std::string& args) {
    auto [remote, local] = splitTwoArgs(args);
    client_.download_file(remote, local);
    std::cout << "File downloaded successfully\n";
}

std::pair<std::string, std::string> UserCommandHandler::splitTwoArgs(const std::string& args) {
    size_t pos = args.find(' ');
    if (pos == std::string::npos) throw std::invalid_argument("Invalid arguments");
    return {args.substr(0, pos), args.substr(pos + 1)};
}