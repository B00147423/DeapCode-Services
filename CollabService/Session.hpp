#pragma once
#include <boost/beast/websocket.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <memory>
#include <vector>
#include <mutex>
#include "Types.hpp"
#include "CRDT.hpp"

class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(boost::asio::ip::tcp::socket sock);
    void run();

    // Send raw text frame
    void send(const std::string& msg);

    // Static broadcast helper
    static void broadcast(const std::string& msg);

private:
    using ws_stream = boost::beast::websocket::stream<boost::asio::ip::tcp::socket>;

    ws_stream              _ws;
    boost::beast::flat_buffer _buffer;
    CRDT                   _crdt;

    // store weak refs so we don't keep dead sessions alive
    static inline std::vector<std::weak_ptr<Session>> _sessions;
    static inline std::mutex _sessions_mtx;

    void doRead();
    void onRead(boost::beast::error_code ec, std::size_t bytes_transferred);
};
