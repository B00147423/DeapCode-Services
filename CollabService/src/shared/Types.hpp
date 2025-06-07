// ===================== Types.hpp =====================
#pragma once
#include <string>
#include <unordered_map>
#include <vector>

enum class MessageType {
    JOIN,
    LEAVE,
    CODE_EDIT,
    CHAT
};

struct Message {
    MessageType type;
    std::string userId;
    std::string roomId;
    std::string content;
};

// Define the PerSocketData struct used by uWebSockets
struct PerSocketData {
    std::string userId;
    std::string roomId;
};