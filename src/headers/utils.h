
#pragma once

#include <map>
#include <string>
#include <variant>

#include "node.h"
#include "nodeApi.h"
#include "log/ErrorStorage.h"


using ComplexNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;


Node createNode(const NodeApi& node)
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
    return Node{ .value = nullptr };
}


template <typename T, typename U>
concept ConceptNodeType = std::same_as<T, Node> && std::same_as<U, size_t> ||
                          std::same_as<T, NodeApi> && std::same_as<U, std::string>;

template <typename T, typename U> requires ConceptNodeType<T, U>
NodeType getNodeType(const T& node)
{
    if (std::holds_alternative<std::map<U, T>>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<std::vector<T>>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


template <typename T>
T checkComplexNode(ComplexNodePtr node)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return nullptr;
    }
    if (std::holds_alternative<T>(node)) {
        return std::get<T>(node);
    }

    if constexpr (std::same_as<T, ObjectNode>) {
        ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_OBJECT);
        return nullptr;
    }
    ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_ARRAY);
    return nullptr;
}


template <typename T>
concept ConceptComplexNode = std::same_as<T, ObjectNode> || std::same_as<T, ArrayNode>;

template <typename T> requires ConceptComplexNode<T>
T& createNodeInObjectAndGet(ObjectNode& obj, uint32_t keyID)
{
    obj.emplace(keyID, T());
    return std::get<T>(obj.at(keyID).value);
}

template <typename T> requires ConceptComplexNode<T>
T& createNodeInArrayAndGet(ArrayNode& arr)
{
    arr.emplace_back(T());
    return std::get<T>(arr.back().value);
}

