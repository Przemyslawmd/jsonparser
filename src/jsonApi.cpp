
#include "jsonApi.h"

#include <iostream>

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"


using ObjectNodeApi = std::map<std::string, Node>;
using ArrayNodeApi = std::vector<Node>;


JsonApi::JsonApi()
{
    keyMapper = std::make_unique<KeyMapper>();
    utils = std::make_unique < Utils>();
    manager = std::make_unique<Manager>();
}


bool JsonApi::parseJsonString(const std::string& jsonString)
{
    if (manager->parseJsonString(jsonString) == false) {
        error = manager->getError();
        return false;
    }
    return true;
}


std::string JsonApi::parseObjectToJsonString()
{
    return manager->parseObjectToString();
}


bool JsonApi::loadObjectJson(const Node& node)
{
    return manager->loadObjectJson(node);
}


void JsonApi::clear()
{
    root.reset();
    error.reset();
    keyMapper->clear();
}


bool JsonApi::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node& node)
{
    return manager->addNodeIntoObject(path, keyStr, node);
}


bool JsonApi::addNodeIntoArray(const std::vector<Path>& path, const Node& node)
{
    return manager->addNodeIntoArray(path, node);
}


bool JsonApi::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const Node& node)
{
    return manager->insertNodeIntoArray(path, index, node);
}


bool JsonApi::changeNodeInObject(const std::vector<Path>& path, const std::string& key, const Node& newNode)
{
    return manager->changeNodeInObject(path, key, newNode);
}


bool JsonApi::changeNodeInArray(const std::vector<Path>& path, size_t index, const Node& node)
{
    return manager->changeNodeInArray(path, index, node);
}


bool JsonApi::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    return manager->removeNodeFromObject(path, keyStr);
}


bool JsonApi::removeNodeFromArray(const std::vector<Path>& path, size_t index)
{
    return manager->removeNodeFromArray(path, index);
}


ErrorCode JsonApi::getErrorCode()
{
    if (error == nullptr) {
        return ErrorCode::NO_ERROR;
    }
    return error->getErrorCode();
}

/*******************************************************************/
/* PRIVATE *********************************************************/


template <typename T>
bool JsonApi::validateNodeType(ComplexNodePtr node, ErrorCode errorCode)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return false;
    }
    if (std::holds_alternative<T>(node) == false) {
        error = std::make_unique<Error>(errorCode);
        return false;
    }
    return true;
}

template <typename T>
T* JsonApi::putIntoObjectAndGet(ObjectNode* obj, uint32_t keyID)
{
    obj->emplace(std::make_pair(keyID, T()));
    return &(std::get<T>(obj->at(keyID).value));
}


template <typename T>
T* JsonApi::putIntoArrayAndGet(ArrayNode* arr)
{
    arr->emplace_back(T());
    return &(std::get<T>(arr->back().value));
}

