
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


bool JsonApi::parseJsonString(const std::string& jsonString)
{
    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    if (tokens == nullptr) {
        result = preparser->getError();
        return false;
    }

    const auto validator = std::make_unique<Validator>();
    result = validator->validate(*tokens);
    if (result != Result::OK) {
        return false;
    }

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>();
    root = parser->parseTokens(*tokens);
    return true;
}


std::string JsonApi::parseObjectToJsonString()
{
    if (isRootEmpty()) {
        return {};
    }
    auto writer = std::make_unique<Writer>();
    return { writer->createJsonString(root.get()) };
}


InnerNodePtr JsonApi::getNode(const std::vector<Indicator>& indicators)
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

    for (const auto& indicator : indicators) {
        if (lastType == InnerNodeType::OBJECT && std::holds_alternative<std::string>(indicator)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);

            const auto& key = std::get<std::string>(indicator);
            if (obj->contains(key) == false) {
                result = Result::API_NOT_KEY_IN_MAP;
                return nullptr;
            }

            Node* node = &obj->at(key);
            getNextNode(&nodePtr, node, lastType);
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<size_t>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);

            size_t index = std::get<size_t>(indicator);
            if (index >= arr->size()) {
                result = Result::API_INDEX_OUT_OF_ARRAY;
                return nullptr;
            }

            Node* node = &arr->at(index);
            getNextNode(&nodePtr, node, lastType);
        }
        else {
            result = Result::API_INCONSISTENT_DATA;
            return nullptr;
        }
    }
    return nodePtr;
}


bool JsonApi::changeNodeInObject(const std::vector<Indicator>& keys, const std::string& key, Node node)
{
    ObjectNode* obj = getObjectAndCheckKey(keys, key);
    if (obj == nullptr) {
        return false;
    }

    obj->at(key) = node;
    return true;
}


bool JsonApi::changeNodeInArray(const std::vector<Indicator>& keys, int index, Node node)
{
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
    if (validateNodeType<ObjectNode*>(node, Result::API_NODE_NOT_OBJECT) == false) {
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
    if (validateNodeType<ArrayNode*>(node, Result::API_NODE_NOT_ARRAY) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    arr->emplace_back(newNode);
    return true;
}


bool JsonApi::insertNodeIntoArray(const std::vector<Indicator>& keys, int index   , Node node)
{
    ArrayNode* arr = getArrayAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    arr->insert(arr->begin() + index, node);
    return true;
}


bool JsonApi::removeNodeFromObject(const std::vector<Indicator>& keys, const std::string& key)
{
    ObjectNode* obj = getObjectAndCheckKey(keys, key);
    if (obj == nullptr) {
        return false;
    }

    obj->erase(key);
    return true;
}


bool JsonApi::removeNodeFromArray(const std::vector<Indicator>& keys, int index)
{
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


Result JsonApi::getLastError()
{
    return result;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

bool JsonApi::isRootEmpty()
{
    if (root == nullptr) {
        result = Result::API_EMPTY;
        return true;
    }
    return false;
}


template <typename T>
bool JsonApi::validateNodeType(InnerNodePtr node, Result potentialError)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return false;
    }
    if (std::holds_alternative<T>(node) == false) {
        result = potentialError;
        return false;
    }
    return true;
}


ArrayNode* JsonApi::getArrayAndCheckIndex(const std::vector<Indicator>& keys, int index)
{
    if (index < 0) {
        result = Result::API_INDEX_NEGATIVE;
        return nullptr;
    }
    
    if (isRootEmpty()) {
        return nullptr;
    }

    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ArrayNode*>(node, Result::API_NODE_NOT_ARRAY) == false) {
        return nullptr;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    if (index > arr->size()) {
        result = Result::API_INDEX_OUT_OF_ARRAY;
        return nullptr;
    }
    return arr;
}


ObjectNode* JsonApi::getObjectAndCheckKey(const std::vector<Indicator>& keys, const std::string& key)
{
    if (isRootEmpty()) {
        return nullptr;
    }

    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ObjectNode*>(node, Result::API_NODE_NOT_OBJECT) == false) {
        return nullptr;
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);
    if (obj->contains(key) == false) {
        result = Result::API_NOT_KEY_IN_MAP;
        return nullptr;
    }
    return obj;
}