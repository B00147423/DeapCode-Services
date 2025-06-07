// ===================== RoomManager.cpp =====================
#include "RoomManager.hpp"

Room* RoomManager::getOrCreateRoom(const std::string& roomId) {
    if (rooms.find(roomId) == rooms.end()) {
        rooms[roomId] = new Room(roomId);
    }
    return rooms[roomId];
}

void RoomManager::removeRoomIfEmpty(const std::string& roomId) {
    auto it = rooms.find(roomId);
    if (it != rooms.end() && it->second->isEmpty()) {
        delete it->second;
        rooms.erase(it);
    }
}
