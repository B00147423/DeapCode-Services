// ===================== main.cpp =====================
#include <uwebsockets/App.h>
#include <iostream>
#include "room/RoomManager.hpp"
#include "shared/Types.hpp"
#include <nlohmann/json.hpp>
#include "ws/WebSocketHandlers.hpp"
using json = nlohmann::json;

RoomManager roomManager;


int main() {
    uWS::TemplatedApp<false>().ws<PerSocketData>("/*", {
        .open = [](auto* ws) {
            std::cout << "Client connected\n";
        },
        .message = [](auto* ws, std::string_view msg, uWS::OpCode) {
            handleMessage(ws, msg);
        },
        .close = [](auto* ws, int code, std::string_view reason) {
            std::cout << "Client disconnected\n";
            handleClose(ws);
        }
        }).listen(9001, [](auto* token) {
            if (token) {
                std::cout << "Server running on port 9001\n";
            }
            }).run();

    return 0;
}