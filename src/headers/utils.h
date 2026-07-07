
#ifndef JX_UTILS_H
#define JX_UTILS_H

#include <map>
#include <string>
#include <variant>

#include "node.h"
#include "nodeApi.h"
#include "log/ErrorStorage.h"


using ComplexNode = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;


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
concept ConceptNodeType = std::is_same<T, Node>::value && std::is_same<U, size_t>::value ||
                          std::is_same<T, NodeApi>::value && std::is_same<U, std::string>::value;

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
T checkComplexNode(ComplexNode node)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return nullptr;
    }
    if (std::holds_alternative<T>(node) == false) {
        if (std::is_same<T, ObjectNode*>::value) {
            ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_OBJECT);
        }
        else if (std::is_same<T, ArrayNode*>::value) {
            ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_ARRAY);
        }
        return nullptr;
    }
    return std::get<T>(node);
}


template <typename T>
T& putIntoObjectAndGet(ObjectNode& obj, uint32_t keyID)
{
    obj.emplace(keyID, T());
    return std::get<T>(obj.at(keyID).value);
}


template <typename T>
T& putIntoArrayAndGet(ArrayNode& arr)
{
    arr.emplace_back(T());
    return std::get<T>(arr.back().value);
}

#endif

