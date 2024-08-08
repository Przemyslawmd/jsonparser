
#include "utils.h"

#include <string>
#include <variant>

using ObjectNodeApi = std::map<std::string, Node>;
using ArrayNodeApi = std::vector<Node>;


NodeInternal Utils::getNodeInternal(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        return NodeInternal{ .value = std::get<std::string>(node.value) };
    }
    if (std::holds_alternative<int64_t>(node.value)) {
        return NodeInternal{ .value = std::get<int64_t>(node.value) };
    }
    if (std::holds_alternative<double>(node.value)) {
        return NodeInternal{ .value = std::get<double>(node.value) };
    }
    if (std::holds_alternative<bool>(node.value)) {
        return NodeInternal{ .value = std::get<bool>(node.value) };
    }
    if (std::holds_alternative<nullptr_t>(node.value)) {
        return NodeInternal{ .value = nullptr };
    }
    // TODO : Error if change not for simple value 
}


NodeType Utils::getNodeType(const Node& node)
{
    if (std::holds_alternative<ObjectNodeApi>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNodeApi>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


NodeType Utils::getNodeInternalType(const NodeInternal& node)
{
    if (std::holds_alternative<ObjectNode>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNode>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}

