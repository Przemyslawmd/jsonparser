
#include "jsonApi.h"


JsonApi::JsonApi()
{
    manager = std::make_unique<Manager>();
}


bool JsonApi::parseJsonString(const std::string& json)
{
    return manager->parseJsonString(json);
}


std::optional<std::string> JsonApi::parseJsonObjectToString()
{
    return manager->parseObjectToString();
}


bool JsonApi::loadJsonObject(const Node& node)
{
    return manager->loadJsonObject(node);
}


void JsonApi::clear()
{
    manager->clear();
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


bool JsonApi::changeNodeInObject(const std::vector<Path>& path, const std::string& key, const Node& node)
{
    return manager->changeNodeInObject(path, key, node);
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


const std::vector<Error>& JsonApi::getErrors()
{
    return manager->getErrors();
}

