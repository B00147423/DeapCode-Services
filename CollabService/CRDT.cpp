#include "CRDT.hpp"
#include <mutex>

std::string CRDT::apply(const Operation& op) {
    std::lock_guard<std::mutex> lk(_mu);
    if (op.type == OpType::Insert &&
        op.position >= 0 &&
        op.position <= static_cast<int>(_text.size()))
    {
        _text.insert(_text.begin() + op.position, op.character);
    }
    else if (op.type == OpType::Delete &&
        op.position >= 0 &&
        op.position < static_cast<int>(_text.size()))
    {
        _text.erase(_text.begin() + op.position);
    }
    return _text;
}

std::string CRDT::state() {
    std::lock_guard<std::mutex> lk(_mu);
    return _text;
}
