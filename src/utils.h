
#ifndef JSONPARSER_UTILS_H
#define JSONPARSER_UTILS_H

#include <map>
#include <string>
#include <variant>

#include "defines.h"
#include "Node.h"
#include "NodeApi.h"


enum class NodeType {
    SIMPLE,
    OBJECT,
    ARRAY
};


static Node getNodeInternal(const NodeApi& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        return Node{ .value = std::get<std::string>(node.value) };
    }
    if (std::holds_alternative<int64_t>(node.value)) {
        return Node{ .value = std::get<int64_t>(node.value) };
    }
    if (std::holds_alternative<double>(node.value)) {
        return Node{ .value = std::get<double>(node.value) };
    }
    if (std::holds_alternative<bool>(node.value)) {
        return Node{ .value = std::get<bool>(node.value) };
    }
    if (std::holds_alternative<nullptr_t>(node.value)) {
        return Node{ .value = nullptr };
    }
    return {};
}


static NodeType getNodeType(const NodeApi& node)
{
    if (std::holds_alternative<std::map<std::string, NodeApi>>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<std::vector<NodeApi>>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


static NodeType getNodeInternalType(const Node& node)
{
    if (std::holds_alternative<std::map<size_t, Node>>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<std::vector<Node>>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


#endif

