
#include "jsonApi.h"

#include <iostream>

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"


enum class InnerNodeType
{
    OBJECT,
    ARRAY
};


JsonApi::JsonApi()
{
    keyMapper = std::make_unique<KeyMapper>();
}


bool JsonApi::parseJsonString(const std::string& jsonString)
{
    error.reset();
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_EMPTY);
        return false;
    }

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    if (tokens == nullptr) {
        error = preparser->getError();
        return false;
    }

    const auto validator = std::make_unique<Validator>();
    if (validator->validate(*tokens) == false) {
        error = validator->getError();
        return false;
    }

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>(*keyMapper.get());
    root = parser->parseTokens(*tokens);
    return true;
}


std::string JsonApi::parseObjectToJsonString()
{
    if (isRootEmpty()) {
        return {};
    }
    auto writer = std::make_unique<Writer>(*keyMapper.get());
    return { writer->createJsonString(root.get()) };
}

/*
bool JsonApi::loadObject(std::unique_ptr<ObjectNode> objectNode)
{
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_EMPTY);
        return false;
    }
    root = std::move(objectNode);
    return true;
}


void JsonApi::clear()
{
    root.reset();
    error.reset();
}
*/

bool JsonApi::changeNodeInObject(const std::vector<Indicator>& path, const std::string& key, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ObjectNode* obj = getObjectAndCheckKey(path, key);
    if (obj == nullptr) {
        return false;
    }

    if (getNodeType(newNode) == NodeType::SIMPLE) {
        size_t keyId = keyMapper->getIdKey(key, obj->begin()->first).value();
        obj->at(keyId) = newNode;
        return true;
    }

}

/*
bool JsonApi::changeNodeInArray(const std::vector<Indicator>& keys, size_t index, Node node)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    arr->at(index) = node;
    return true;
}
*/

bool JsonApi::addNodeIntoObject(const std::vector<Indicator>& path, const std::string& key, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    InnerNodePtr objNode = getNode(path);
    if (validateNodeType<ObjectNode*>(objNode, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return false;
    }

    if (getNodeType(newNode) == NodeType::SIMPLE) {
        ObjectNode* obj = std::get<ObjectNode*>(objNode);
        size_t newId = keyMapper->putKeyIntoExistingMap(key, obj->begin()->first);
        obj->emplace(std::make_pair(newId, newNode));
        return true;
    }
    return false;
}

/*
bool JsonApi::addNodeIntoArray(const std::vector<Indicator>& keys, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ArrayNode*>(node, ErrorCode::API_NODE_NOT_ARRAY) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    arr->emplace_back(newNode);
    return true;
}


bool JsonApi::insertNodeIntoArray(const std::vector<Indicator>& keys, int index   , Node node)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    arr->insert(arr->begin() + index, node);
    return true;
}


bool JsonApi::removeNodeFromObject(const std::vector<Indicator>& keys, const std::string& key)
{
    if (isRootEmpty()) {
        return false;
    }

    ObjectNode* obj = getObjectAndCheckKey(keys, key);
    if (obj == nullptr) {
        return false;
    }

    obj->erase(key);
    return true;
}


bool JsonApi::removeNodeFromArray(const std::vector<Indicator>& keys, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    arr->erase(arr->begin() + index);
    return true;
}


ObjectNode* JsonApi::getRoot()
{
    return root.get();
}
*/

ErrorCode JsonApi::getErrorCode()
{
    if (error == nullptr) {
        return ErrorCode::NO_ERROR;
    }
    return error->getErrorCode();
}


/*******************************************************************/
/* PRIVATE *********************************************************/

bool JsonApi::isRootEmpty()
{
    if (root == nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_EMPTY);
        return true;
    }
    return false;
}


InnerNodePtr JsonApi::getNode(const std::vector<Indicator>& path)
{
    if (isRootEmpty()) {
        return nullptr;
    }

    InnerNodePtr nodePtr = root.get();
    InnerNodeType lastType = InnerNodeType::OBJECT;

    const auto getNextNode = [](InnerNodePtr* nodePtr, Node* node, InnerNodeType& lastType) 
    {
        if (std::holds_alternative<ObjectNode>(node->value)) {
            *nodePtr = std::get_if<ObjectNode>(&node->value);
            lastType = InnerNodeType::OBJECT;
        }
        else if (std::holds_alternative<ArrayNode>(node->value)) {
            *nodePtr = std::get_if<ArrayNode>(&node->value);
            lastType = InnerNodeType::ARRAY;
        }
    };

    for (const auto indicator : path) {
        if (lastType == InnerNodeType::OBJECT && std::holds_alternative<std::string>(indicator)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);
            const auto& keyStr = std::get<std::string>(indicator);
            size_t keyId = keyMapper->getIdKey(keyStr, obj->begin()->first).value();
            if (obj->contains(keyId) == false) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }

            Node* node = &obj->at(keyId);
            getNextNode(&nodePtr, node, lastType);
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<size_t>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);
            size_t index = std::get<size_t>(indicator);
            if (index >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            Node* node = &arr->at(index);
            getNextNode(&nodePtr, node, lastType);
        }
        else {
            error = std::make_unique<Error>(ErrorCode::API_INCONSISTENT_DATA);
            return nullptr;
        }
    }
    return nodePtr;
}


template <typename T>
bool JsonApi::validateNodeType(InnerNodePtr node, ErrorCode potentialError)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return false;
    }
    if (std::holds_alternative<T>(node) == false) {
        error = std::make_unique<Error>(potentialError);
        return false;
    }
    return true;
}


ArrayNode* JsonApi::getArrayAndCheckIndex(const std::vector<Indicator>& path, size_t index)
{
    InnerNodePtr node = getNode(path);
    if (validateNodeType<ArrayNode*>(node, ErrorCode::API_NODE_NOT_ARRAY) == false) {
        return nullptr;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    if (index > arr->size()) {
        error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
        return nullptr;
    }
    return arr;
}


ObjectNode* JsonApi::getObjectAndCheckKey(const std::vector<Indicator>& path, const std::string& key)
{
    InnerNodePtr node = getNode(path);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return nullptr;
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);
    size_t keyId = keyMapper->getIdKey(key, obj->begin()->first).value();
    if (obj->contains(keyId) == false) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
        return nullptr;
    }
    return obj;
}


NodeType JsonApi::getNodeType(Node& node)
{
    if (std::holds_alternative<ObjectNode>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNode>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}