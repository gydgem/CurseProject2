#include "Client.h"
#include "UserCommandHandler.h"

int main() {
    try {
        boost::asio::io_context io_context;
        Client client(io_context, "localhost", 1234);
        UserCommandHandler handler(client);
        std::cout << "Connected to server. Type 'help' for commands list.\n";

        std::string input;
        while (true) {
            std::cout << "> ";
            std::getline(std::cin, input);
            if (input.empty()) continue;

            if (input == "exit") break;

            if (input == "help") {
                UserCommandHandler::printHelp();
                continue;
            }

            try {
                handler.handleCommand(input);
            } catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << "\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << std::endl;
    }
    return 0;
}