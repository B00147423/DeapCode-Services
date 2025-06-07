
// ===================== Room.cpp =====================
#include "Room.hpp"
#include <iostream>

Room::Room(const std::string& id) : id(id) {}

void Room::addClient(const std::string& userId, uWS::WebSocket<false, true, PerSocketData>* ws) {
    clients[userId] = ws;
    std::cout << userId << " joined room " << id << "\n";
}

void Room::removeClient(const std::string& userId) {
    clients.erase(userId);
    std::cout << userId << " left room " << id << "\n";
}

void Room::broadcast(const std::string& message) {
    for (auto& [uid, ws] : clients) {
        ws->send(message, uWS::OpCode::TEXT);
    }
}

bool Room::isEmpty() const {
    return clients.empty();
}