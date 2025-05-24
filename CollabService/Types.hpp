#pragma once
#include <string>
#include <nlohmann/json.hpp>

enum class OpType { Insert, Delete };
NLOHMANN_JSON_SERIALIZE_ENUM(OpType, {
  {OpType::Insert, "Insert"},
  {OpType::Delete, "Delete"}
    })

    struct Operation {
    OpType    type;
    int       position;
    char      character;   // only for Insert
    uint64_t  clientId;
    uint64_t  seqNum;      // for ordering
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        Operation,
        type, position, character, clientId, seqNum
    )
};

struct Message {
    std::string docId;
    std::string kind;     // e.g. "join", "op", "state"
    nlohmann::json payload;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        Message,
        docId, kind, payload
    )
};
