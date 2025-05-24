#include <boost/beast/websocket.hpp>
#include <boost/beast/core/buffers_to_string.hpp>
#include <boost/asio.hpp>            // for boost::asio::buffer()
#include <boost/asio/buffer.hpp>     // sometimes needed on MSVC
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>
#include "Session.hpp"       
#include <array>                     // for std::array


using tcp = boost::asio::ip::tcp;
using json = nlohmann::json;

Session::Session(tcp::socket sock)
    : _ws(std::move(sock))
{
}

void Session::run() {
    auto self = shared_from_this();
    _ws.accept([self](boost::beast::error_code ec) {
        if (ec) {
            spdlog::error("WebSocket accept error: {}", ec.message());
            return;
        }
        {
            std::lock_guard<std::mutex> lk(_sessions_mtx);
            _sessions.push_back(self);
        }
        spdlog::info("Client joined, total sessions: {}", _sessions.size());
        self->doRead();
        });
}

void Session::doRead() {
    auto self = shared_from_this();
    _ws.async_read(_buffer,
        [self](auto ec, std::size_t bytes) {
            self->onRead(ec, bytes);
        }
    );
}

void Session::onRead(boost::beast::error_code ec, std::size_t) {
    if (ec) {
        spdlog::warn("Read error: {}", ec.message());
        return;
    }

    // Convert buffer to string
    std::string txt = boost::beast::buffers_to_string(_buffer.data());
    _buffer.consume(_buffer.size());

    // Parse incoming message
    auto msg = json::parse(txt).get<Message>();

    if (msg.kind == "op") {
        // apply the operation
        Operation op = msg.payload.get<Operation>();
        std::string newState = _crdt.apply(op);

        // broadcast new state
        Message out;
        out.docId = msg.docId;
        out.kind = "state";
        out.payload = newState;
        std::string serialized = json(out).dump();
        broadcast(serialized);
    }

    doRead();
}


void Session::send(const std::string& msg) {
    _ws.text(true);
    boost::system::error_code ec;

    // Use asio::buffer directly, Beast accepts this just fine
    _ws.write(boost::asio::buffer(msg), ec);

    if (ec) {
        spdlog::error("WebSocket write failed: {}", ec.message());
    }
}

void Session::broadcast(const std::string& msg) {
    std::lock_guard<std::mutex> lk(_sessions_mtx);
    for (auto it = _sessions.begin(); it != _sessions.end(); ) {
        if (auto sp = it->lock()) {
            sp->send(msg);
            ++it;
        }
        else {
            it = _sessions.erase(it);
        }
    }
}
