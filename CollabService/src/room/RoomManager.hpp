// ===================== RoomManager.hpp =====================
#pragma once
#include "Room.hpp"
#include <unordered_map>
#include <string>

class RoomManager {
public:
    Room* getOrCreateRoom(const std::string& roomId);
    void removeRoomIfEmpty(const std::string& roomId);
private:
    std::unordered_map<std::string, Room*> rooms;
};
