
#ifndef JSONPARSER_UTILS_H
#define JSONPARSER_UTILS_H

#include <variant>

#include <defines.h>
#include <NodeValue.h>


enum class NodeType {
    SIMPLE,
    OBJECT,
    ARRAY
};


static NodeInternal getNodeInternal(const Node& node)
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
    return {};
}


static NodeType getNodeType(const Node& node)
{
    if (std::holds_alternative<std::map<std::string, Node>>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<std::vector<Node>>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


static NodeType getNodeInternalType(const NodeInternal& node)
{
    if (std::holds_alternative<std::map<size_t, NodeInternal>>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<std::vector<NodeInternal>>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


#endif

