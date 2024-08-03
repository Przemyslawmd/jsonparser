
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
        obj->at(keyId) = getNodeInternalFromNode(newNode);
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

    ObjectNode* obj = std::get<ObjectNode*>(objNode);
    size_t newId = keyMapper->putKeyIntoExistingMap(key, obj->begin()->first);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        obj->emplace(std::make_pair(newId, getNodeInternalFromNode(newNode)));
        return true;
    }
    else if (newNodeType == NodeType::OBJECT) {
        obj->emplace(std::make_pair(newId, ObjectNode()));
        ObjectNode* objNew = &(std::get<ObjectNode>(obj->at(newId).value));
        addNodeIntoObjectInternally(objNew, newNode);
    }
    return true;
}


bool JsonApi::addNodeIntoObjectInternally(ObjectNode* currentObject, Node newNode)
{
    size_t mapID = keyMapper->getMaxMapID() + (1 << 16);
    size_t nodeID = 0;
    const size_t BIT_MASK = 0b11111111111111110000000000000000;
    for (auto& [key, val] : std::get<ObjectNodeExternal>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        size_t itemID = (mapID & BIT_MASK) + nodeID;
        keyMapper->putKey(key, itemID);
        if (newNodeType == NodeType::SIMPLE) {
            currentObject->emplace(std::make_pair(itemID, getNodeInternalFromNode(val)));
        }
        else if (newNodeType == NodeType::OBJECT) {
            currentObject->emplace(std::make_pair(itemID, ObjectNode()));
            auto* objNew = &(std::get<ObjectNode>(currentObject->at(itemID).value));
            addNodeIntoObjectInternally(objNew, val);
        }
        nodeID++;
    }
    return true;
}


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
    NodeType newNodeType = getNodeType(newNode);
    
    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(getNodeInternalFromNode(newNode));
    }
    return true;
}


bool JsonApi::insertNodeIntoArray(const std::vector<Indicator>& keys, int index, Node node)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeType(node);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, getNodeInternalFromNode(node));
    }
    return true;
}

/*
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

    const auto getNextNode = [](InnerNodePtr* nodePtr, NodeInternal* node, InnerNodeType& lastType) 
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

            NodeInternal* node = &obj->at(keyId);
            getNextNode(&nodePtr, node, lastType);
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<size_t>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);
            size_t index = std::get<size_t>(indicator);
            if (index >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            NodeInternal* node = &arr->at(index);
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
    if (std::holds_alternative<ObjectNodeExternal>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNodeExternal>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


NodeInternal JsonApi::getNodeInternalFromNode(Node& node)
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
}
