
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

    const auto parser = std::make_unique<Parser>();
    root = parser->parseTokens(*tokens, keyMapper.get());
    return true;
}


std::string JsonApi::parseObjectToJsonString()
{
    if (isRootEmpty()) {
        return {};
    }
    auto writer = std::make_unique<Writer>();
    return { writer->createJsonString(root.get(), keyMapper.get()) };
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


bool JsonApi::changeNodeInObject(const std::vector<Indicator>& keys, const std::string& key, Node node)
{
    if (isRootEmpty()) {
        return false;
    }

    ObjectNode* obj = getObjectAndCheckKey(keys, key);
    if (obj == nullptr) {
        return false;
    }

    obj->at(key) = node;
    return true;
}


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


bool JsonApi::addNodeIntoObject(const std::vector<Indicator>& keys, const std::string& key, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return false;
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);
    obj->emplace(std::make_pair(key, newNode));
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


InnerNodePtr JsonApi::getNode(const std::vector<size_t>& keys)
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

    for (const size_t key : keys) {
        if (lastType == InnerNodeType::OBJECT) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);

            if (obj->contains(key) == false) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }

            Node* node = &obj->at(key);
            getNextNode(&nodePtr, node, lastType);
        }
        else if (lastType == InnerNodeType::ARRAY) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);
            if (key >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            Node* node = &arr->at(key);
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


ArrayNode* JsonApi::getArrayAndCheckIndex(const std::vector<size_t>& keys, size_t index)
{
    InnerNodePtr node = getNode(keys);
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


ObjectNode* JsonApi::getObjectAndCheckKey(const std::vector<size_t>& keys, const size_t key)
{
    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return nullptr;
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);
    if (obj->contains(key) == false) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
        return nullptr;
    }
    return obj;
}

