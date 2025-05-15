#include "Server.h"
#include <iostream>

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 1234);
        std::cout << "Server started on port 1234" << std::endl;
        io_context.run();
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
    return 0;
}