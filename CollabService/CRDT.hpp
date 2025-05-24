#pragma once
#include "Types.hpp"
#include <string>
#include <mutex>

class CRDT {
    std::string _text;
    std::mutex  _mu;
public:
    // Apply an operation, return the new text
    std::string apply(const Operation& op);
    // Get full state snapshot
    std::string state();
};
