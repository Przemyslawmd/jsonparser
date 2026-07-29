
#pragma once

#include <map>
#include <string>
#include <vector>
#include <variant>


enum class NodeType
{
    SIMPLE,
    OBJECT,
    ARRAY
};


struct Node
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<size_t, Node>, std::vector<Node>>;

    Node(const Value& value) : value(value) {}

    Value value;
};


using ObjectNode = std::map<size_t, Node>;
using ArrayNode = std::vector<Node>;

