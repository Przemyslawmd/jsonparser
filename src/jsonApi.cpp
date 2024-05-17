
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


bool jsonApi::parseJsonString(const std::string& jsonString)
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


std::string jsonApi::parseObjectToJsonString(ObjectNode* node)
{
    auto writer = std::make_unique<Writer>();
    return { writer->createJsonString(node) };
}


ObjectNode* jsonApi::getRoot()
{
    return root.get();
}


std::string jsonApi::getNodeType(std::vector<indicator> keys)
{
    return "Test";
}


#include <iostream>

InnerNodePtr jsonApi::getNode(const std::vector<indicator>& indicators)
{
    InnerNodePtr nodePtr = root.get();
    InnerNodeType lastType = InnerNodeType::OBJECT;

    for (const auto& indicator : indicators) {
        if (lastType == InnerNodeType::OBJECT && std::holds_alternative<std::string>(indicator)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);

            const auto& key = std::get<std::string>(indicator);
            if (obj->contains(key) == false) {
                result = Result::API_NOT_KEY_IN_MAP;
                return nullptr;
            }

            Node* node = &obj->at(key);
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodePtr = std::get_if<ObjectNode>(&node->value);
                lastType = InnerNodeType::OBJECT;
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodePtr = std::get_if<ArrayNode>(&node->value);
                lastType = InnerNodeType::ARRAY;
            }
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<size_t>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);

            size_t index = std::get<size_t>(indicator);
            if (index >= arr->size()) {
                result = Result::API_INDEX_OUT_OF_ARRAY;
                return nullptr;
            }

            Node* node = &arr->at(index);
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodePtr = std::get_if<ObjectNode>(&node->value);
                InnerNodeType::OBJECT;
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodePtr = std::get_if<ArrayNode>(&node->value);
                lastType = InnerNodeType::ARRAY;
            }
        }
        else {
             return nullptr;
        }
    }
    return nodePtr;
}


bool jsonApi::changeNodeValue(const std::vector<indicator>& keys, Node node)
{
    std::vector<indicator> keysToGetNode{ keys };
    keysToGetNode.pop_back();
    InnerNodePtr innerNodePtr = getNode(keysToGetNode);

    if (std::holds_alternative<nullptr_t>(innerNodePtr)) {
        return false;
    }

    if (std::holds_alternative<ObjectNode*>(innerNodePtr)) {
        std::get<ObjectNode*>(innerNodePtr)->at(std::get<std::string>(keys.back())) = node;
    }
    else if (std::holds_alternative<ArrayNode*>(innerNodePtr)) {
        std::get<ArrayNode*>(innerNodePtr)->at(std::get<size_t>(keys.back())) = node;
    }
    return true;

}


Result jsonApi::getLastError()
{
    return result;
}

