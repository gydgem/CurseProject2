#include "Server.h"
#include "iostream"
#include <thread>

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 1234);

        std::cout << "Server started on port 1234\n";
        std::cout << "Type 'help' for available commands\n";

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
                work_guard = boost::asio::make_work_guard(io_context);

        const int threads_num = 4;
        std::vector<std::thread> threads;
        for (int i = 0; i < threads_num; ++i) {
            threads.emplace_back([&io_context]() { io_context.run(); });
        }

        for (auto &t: threads) {
            if (t.joinable()) t.join();
        }

        std::cout << "Server stopped\n";
    } catch (const std::exception &ex) {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
    return 0;
}