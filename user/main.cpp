#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

class User {
public:
    User(boost::asio::io_context& io_context, const std::string& host, short port)
            : socket_(io_context) {
        tcp::resolver resolver(io_context);
        boost::asio::connect(socket_, resolver.resolve(host, std::to_string(port)));
    }

    std::string send_request(const std::string& request) {
        boost::asio::write(socket_, boost::asio::buffer(request + "\n"));

        boost::asio::streambuf buffer;
        boost::asio::read_until(socket_, buffer, '\n');

        std::istream is(&buffer);
        std::string response;
        std::getline(is, response);

        return response;
    }

private:
    tcp::socket socket_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        User user(io_context, "localhost", 1234);
        std::cout << "Connected to server. Type 'exit' to quit.\n";

        while (true) {
            std::cout << "Enter message: ";
            std::string message;
            std::getline(std::cin, message);

            if (message == "exit") break;

            std::string response = user.send_request(message);
            std::cout << "Server response: " << response << std::endl;
        }
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl;
    }
    return 0;
}