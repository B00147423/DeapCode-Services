// WebSocketHandlers.cpp
#include "WebSocketHandlers.hpp"
#include "../room/RoomManager.hpp"
#include "../shared/Types.hpp"
#include <nlohmann/json.hpp>

extern RoomManager roomManager;  // declared in main.cpp

void handleMessage(uWS::WebSocket<false, true, PerSocketData>* ws, std::string_view msg) {
    using json = nlohmann::json;
    json j = json::parse(msg);

    Message message;
    message.type = static_cast<MessageType>(j["type"].get<int>());
    message.userId = j["userId"];
    message.roomId = j["roomId"];
    message.content = j["content"];

    ws->getUserData()->userId = message.userId;
    ws->getUserData()->roomId = message.roomId;

    Room* room = roomManager.getOrCreateRoom(message.roomId);

    switch (message.type) {
    case MessageType::JOIN:
        room->addClient(message.userId, ws);
        room->broadcast(message.userId + " joined the room.");
        break;
    case MessageType::LEAVE:
        room->removeClient(message.userId);
        room->broadcast(message.userId + " left the room.");
        roomManager.removeRoomIfEmpty(message.roomId);
        break;
    case MessageType::CHAT:
    case MessageType::CODE_EDIT:
        room->broadcast(message.userId + ": " + message.content);
        break;
    default:
        std::cerr << "Unknown message type\n";
    }
}
void handleClose(uWS::WebSocket<false, true, PerSocketData>* ws) {
    std::string userId = ws->getUserData()->userId;
    std::string roomId = ws->getUserData()->roomId;
    Room* room = roomManager.getOrCreateRoom(roomId);
    room->removeClient(userId);
    room->broadcast(userId + " left the room.");
    roomManager.removeRoomIfEmpty(roomId);
}
