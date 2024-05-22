
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
    if (root == nullptr) {
        result = Result::API_EMPTY;
        return {};
    }
    auto writer = std::make_unique<Writer>();
    return { writer->createJsonString(root.get()) };
}


ObjectNode* JsonApi::getRoot()
{
    return root.get();
}


std::string JsonApi::getNodeType(const std::vector<Indicator>& keys)
{
    return "Test";
}


InnerNodePtr JsonApi::getNode(const std::vector<Indicator>& indicators)
{
    if (root == nullptr) {
        result = Result::API_EMPTY;
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


bool JsonApi::changeNodeValue(const std::vector<Indicator>& keys, Node node)
{
    if (root == nullptr) {
        result = Result::API_EMPTY;
        return false;
    }

    std::vector<Indicator> keysToGetNode{ keys };
    keysToGetNode.pop_back();
    InnerNodePtr innerNodePtr = getNode(keysToGetNode);

    if (std::holds_alternative<nullptr_t>(innerNodePtr)) {
        return false;
    }

    Indicator lastKey = keys.back();
    if (std::holds_alternative<ObjectNode*>(innerNodePtr) && std::holds_alternative<std::string>(lastKey)) {
        ObjectNode* obj = std::get<ObjectNode*>(innerNodePtr);
        const auto& key = std::get<std::string>(lastKey);
        if (obj->contains(key) == false) {
            result = Result::API_NOT_KEY_IN_MAP;
            return false;
        }
        obj->at(key) = node;
    }
    else if (std::holds_alternative<ArrayNode*>(innerNodePtr) && std::holds_alternative<size_t>(lastKey)) {
        ArrayNode* arr = std::get<ArrayNode*>(innerNodePtr);
        size_t index = std::get<size_t>(lastKey);
        if (index >= arr->size()) {
            result = Result::API_INDEX_OUT_OF_ARRAY;
            return false;
        }
        arr->at(index) = node;
    }
    else {
        result = Result::API_INCONSISTENT_DATA;
        return false;
    }
    return true;
}


bool JsonApi::addNodeIntoObject(const std::vector<Indicator>& keys, Node node, const std::string& key)
{
    if (root == nullptr) {
        result = Result::API_EMPTY;
        return {};
    }

    InnerNodePtr innerNodePtr = getNode(keys);

    if (std::holds_alternative<nullptr_t>(innerNodePtr)) {
        return false;
    }
    if (std::holds_alternative<ObjectNode*>(innerNodePtr) == false) {
        result = Result::API_INNER_NODE_NOT_OBJECT;
        return false;
    }

    ObjectNode* obj = std::get<ObjectNode*>(innerNodePtr);
    obj->emplace(std::make_pair(key, node));
    return true;
}


bool JsonApi::addNodeIntoArray(const std::vector<Indicator>& keys, Node node, int index)
{
    if (root == nullptr) {
        result = Result::API_EMPTY;
        return {};
    }

    InnerNodePtr innerNodePtr = getNode(keys);

    if (std::holds_alternative<nullptr_t>(innerNodePtr)) {
        return false;
    }
    if (std::holds_alternative<ArrayNode*>(innerNodePtr) == false) {
        result = Result::API_INNER_NODE_NOT_ARRAY;
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(innerNodePtr);

    if (index < 0) {
        arr->emplace_back(node);
        return true;
    }
    if (index > arr->size()) {
        result = Result::API_INDEX_OUT_OF_ARRAY;
        return false;
    }
    arr->insert(arr->begin() + index, node);
    return true;
}


Result JsonApi::getLastError()
{
    return result;
}

