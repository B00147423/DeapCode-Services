// ===================== Room.hpp =====================
#pragma once
#include "../src/shared/Types.hpp"
#include <uwebsockets/App.h> 
#include <unordered_map>
#include <string>

class Room {
public:
    Room(const std::string& id);
    void addClient(const std::string& userId, uWS::WebSocket<false, true, PerSocketData>* ws);
    void removeClient(const std::string& userId);
    void broadcast(const std::string& message);
    bool isEmpty() const;
private:
    std::string id;
    std::unordered_map<std::string, uWS::WebSocket<false, true, PerSocketData>*> clients;
};