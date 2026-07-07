
#include "jsonApi.h"


JsonApi::JsonApi()
{
    manager = std::make_unique<Manager>();
}

bool JsonApi::parseJsonString(const std::string& file)
{
    return manager->parseJsonString(file);
}

bool JsonApi::parseXmlString(const std::string& file)
{
    return manager->parseXmlString(file);
}

std::optional<std::string> JsonApi::objectToJsonString()
{
    return manager->objectToJsonString();
}

std::optional<std::string> JsonApi::objectToXmlString()
{
    return manager->objectToXmlString();
}

bool JsonApi::loadObject(const NodeApi& node)
{
    return manager->loadObject(node);
}

bool JsonApi::isObject()
{
    return manager->isObject();
}

void JsonApi::clear()
{
    manager->clear();
}

bool JsonApi::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const NodeApi& node)
{
    return manager->addNodeIntoObject(path, keyStr, node);
}

bool JsonApi::addNodeIntoArray(const std::vector<Path>& path, const NodeApi& node)
{
    return manager->addNodeIntoArray(path, node);
}

bool JsonApi::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const NodeApi& node)
{
    return manager->insertNodeIntoArray(path, index, node);
}

bool JsonApi::changeNodeInObject(const std::vector<Path>& path, const std::string& key, const NodeApi& node)
{
    return manager->changeNodeInObject(path, key, node);
}

bool JsonApi::changeNodeInArray(const std::vector<Path>& path, size_t index, const NodeApi& node)
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

