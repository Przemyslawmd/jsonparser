
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


InnerNodePtr jsonApi::getNode(const std::vector<indicator>& indicators)
{
    InnerNodePtr nodePtr = root.get();
    InnerNodeType lastType = InnerNodeType::OBJECT;

    for (const auto& indicator : indicators) {
        if (lastType == InnerNodeType::OBJECT && std::holds_alternative<std::string>(indicator)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);
            const auto& key = std::get<std::string>(indicator);
            if (std::holds_alternative<ObjectNode>(obj->at(key).value)) {
                nodePtr = std::get_if<ObjectNode>(&obj->at(key).value);
                lastType = InnerNodeType::OBJECT;
            } 
            else if (std::holds_alternative<ArrayNode>(obj->at(key).value)) {
                nodePtr = std::get_if<ArrayNode>(&obj->at(key).value);
                lastType = InnerNodeType::ARRAY;
            }
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<int>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);
            int index = std::get<int>(indicator);
            if (std::holds_alternative<ObjectNode>(arr->at(index).value)) {
                nodePtr = std::get_if<ObjectNode>(&arr->at(index).value);
                InnerNodeType::OBJECT;
            }
            else if (std::holds_alternative<ArrayNode>(arr->at(index).value)) {
                nodePtr = std::get_if<ArrayNode>(&arr->at(index).value);
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
        std::get<ArrayNode*>(innerNodePtr)->at(std::get<int>(keys.back())) = node;
    }
    return true;

}

Result jsonApi::getLastError()
{
    return result;
}

