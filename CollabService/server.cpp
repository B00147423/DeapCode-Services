#include <iostream>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include "Session.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

int main() {
    try {
        net::io_context ioc{ 1 };
        tcp::acceptor acceptor{ ioc, {tcp::v4(), 50052} };
        std::cout << "CollabService listening on ws://localhost:50052\n";

        for (;;) {
            tcp::socket socket{ ioc };
            acceptor.accept(socket);
            // create & run a session (self-manages via shared_ptr)
            std::make_shared<Session>(std::move(socket))->run();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}
