
#include "manager.h"

#include <iostream>


#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"

using ObjectNodeApi = std::map<std::string, Node>;
using ArrayNodeApi = std::vector<Node>;


Manager::Manager()
{
    keyMapper = std::make_unique<KeyMapper>();
    utils = std::make_unique <Utils>();
}


bool Manager::parseJsonString(const std::string& jsonString)
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
    if (root != nullptr) {
        return true;
    }
    error = parser->getError();
    return false;
}


std::string Manager::parseObjectToString()
{
    if (isRootEmpty()) {
        return {};
    }
    auto writer = std::make_unique<Writer>(*keyMapper.get());
    auto jsonStr = writer->createJsonString(*root.get());
    if (jsonStr == std::nullopt) {
        error = writer->getError();
        return "";
    }
    return jsonStr.value();
}


bool Manager::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr objNode = getNodeFromPath(path);
    if (validateNodeType<ObjectNode*>(objNode, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return false;
    }

    ObjectNode* obj = std::get<ObjectNode*>(objNode);

    auto optKeyID = keyMapper->createAndPutKeyID(keyStr, obj->begin()->first);
    if (optKeyID == std::nullopt) {
        return false;
    }
    uint32_t keyID = optKeyID.value();

    NodeType newNodeType = utils->getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        obj->emplace(std::make_pair(keyID, utils->getNodeInternal(newNode)));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objNew = putIntoObjectAndGet<ObjectNode>(obj, keyID);
        addObjectNodeInternally(objNew, newNode);
    }
    else if (newNodeType == NodeType::ARRAY) {
        ArrayNode* arrNew = putIntoObjectAndGet<ArrayNode>(obj, keyID);
        addArrayNodeInternally(arrNew, newNode);
    }
    return true;
}


bool Manager::addNodeIntoArray(const std::vector<Path>& path, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr node = getNodeFromPath(path);
    if (validateNodeType<ArrayNode*>(node, ErrorCode::API_NODE_NOT_ARRAY) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    NodeType newNodeType = utils->getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(utils->getNodeInternal(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objNew = putIntoArrayAndGet<ObjectNode>(arr);
        addObjectNodeInternally(objNew, newNode);
    }
    else if (newNodeType == NodeType::ARRAY) {
        ArrayNode* arrNew = putIntoArrayAndGet<ArrayNode>(arr);
        addArrayNodeInternally(arrNew, newNode);
    }
    return true;
}


bool Manager::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = utils->getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, utils->getNodeInternal(newNode));
        return true;
    }
    if (newNodeType == NodeType::OBJECT) {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ObjectNode() });
        ObjectNode* objToAdd = &std::get<ObjectNode>(arr->at(index).value);
        addObjectNodeInternally(objToAdd, newNode);
        return true;
    }
    if (newNodeType == NodeType::ARRAY) {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ArrayNode() });
        ArrayNode* arrToAdd = &std::get<ArrayNode>(arr->at(index).value);
        addArrayNodeInternally(arrToAdd, newNode);
        return true;
    }
}


bool Manager::changeNodeInObject(const std::vector<Path>& path, const std::string& key, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectAndKeyID(path, key);
    if (obj == nullptr) {
        return false;
    }

    NodeType nodeType = utils->getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        obj->at(keyID) = utils->getNodeInternal(newNode);
        return true;
    }

    obj->erase(keyID);
    if (nodeType == NodeType::OBJECT) {
        obj->insert(std::make_pair(keyID, ObjectNode()));
        ObjectNode* objToAdd = &(std::get<ObjectNode>(obj->at(keyID).value));
        addObjectNodeInternally(objToAdd, newNode);
    }
    else if (nodeType == NodeType::ARRAY) {
        obj->insert(std::make_pair(keyID, ArrayNode()));
        ArrayNode* arrToAdd = &(std::get<ArrayNode>(obj->at(keyID).value));
        addArrayNodeInternally(arrToAdd, newNode);
    }
    return true;
}


bool Manager::changeNodeInArray(const std::vector<Path>& path, size_t index, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType nodeType = utils->getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arr->at(index) = utils->getNodeInternal(newNode);
    }
    else if (nodeType == NodeType::OBJECT) {
        arr->at(index) = NodeInternal{ .value = ObjectNode() };
        ObjectNode* objNew = &std::get<ObjectNode>(arr->at(index).value);
        addObjectNodeInternally(objNew, newNode);
        return true;
    }
    else if (nodeType == NodeType::ARRAY) {
        arr->at(index) = NodeInternal{ .value = ArrayNode() };
        ArrayNode* arrNew = &std::get<ArrayNode>(arr->at(index).value);
        addArrayNodeInternally(arrNew, newNode);
        return true;
    }
    return true;
}


bool Manager::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectAndKeyID(path, keyStr);
    if (obj == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = obj->at(keyID);
    NodeType nodeType = utils->getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(obj->at(keyID).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(obj->at(keyID).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    obj->erase(keyID);
    keyMapper->removeKey(keyID);
    return true;
}


bool Manager::removeNodeFromArray(const std::vector<Path>& path, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = arr->at(index);
    NodeType nodeType = utils->getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(arr->at(index).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(arr->at(index).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    arr->erase(arr->begin() + index);
    return true;
}


std::unique_ptr<Error> Manager::getError()
{
    return std::move(error);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

bool Manager::isRootEmpty()
{
    if (root == nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_EMPTY);
        return true;
    }
    return false;
}

ComplexNodePtr Manager::getNodeFromPath(const std::vector<Path>& path)
{
    ComplexNodePtr nodeComplexPtr = root.get();
    if (path.empty()) {
        return nodeComplexPtr;
    }

    NodeType nodeType = NodeType::OBJECT;

    const auto getNextNode = [&nodeType](NodeInternal* node) -> ComplexNodePtr
        {
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodeType = NodeType::OBJECT;
                return std::get_if<ObjectNode>(&node->value);
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodeType = NodeType::ARRAY;
                return std::get_if<ArrayNode>(&node->value);
            }
        };

    for (const auto& pathKey : path) {
        if (nodeType == NodeType::OBJECT && std::holds_alternative<std::string>(pathKey)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodeComplexPtr);
            const auto& keyStr = std::get<std::string>(pathKey);
            std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);

            if (keyID == std::nullopt) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }
            if (obj->contains(keyID.value()) == false) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }
            NodeInternal* node = &obj->at(keyID.value());
            nodeComplexPtr = getNextNode(node);
        }
        else if (nodeType == NodeType::ARRAY && std::holds_alternative<size_t>(pathKey)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodeComplexPtr);
            size_t index = std::get<size_t>(pathKey);

            if (index >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }
            NodeInternal* node = &arr->at(index);
            nodeComplexPtr = getNextNode(node);
        }
        else {
            error = std::make_unique<Error>(ErrorCode::API_INCONSISTENT_DATA);
            return nullptr;
        }
    }
    return nodeComplexPtr;
}


bool Manager::addObjectNodeInternally(ObjectNode* obj, const Node& newNode)
{
    uint32_t mapID = keyMapper->getNextMapID();

    for (auto& [keyStr, val] : std::get<ObjectNodeApi>(newNode.value)) {
        NodeType newNodeType = utils->getNodeType(val);

        auto optKeyID = keyMapper->createAndPutKeyID(keyStr, mapID);
        if (optKeyID == std::nullopt) {
            return false;
        }
        uint32_t keyID = optKeyID.value();

        if (newNodeType == NodeType::SIMPLE) {
            obj->emplace(std::make_pair(keyID, utils->getNodeInternal(val)));
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNew = putIntoObjectAndGet<ObjectNode>(obj, keyID);
            if (addObjectNodeInternally(objNew, val) == false) {
                return false;
            }
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrNew = putIntoObjectAndGet<ArrayNode>(obj, keyID);
            addArrayNodeInternally(arrNew, val);
        }
    }
    return true;
}


bool Manager::addArrayNodeInternally(ArrayNode* arr, const Node& newNode)
{
    for (auto& val : std::get<ArrayNodeApi>(newNode.value)) {
        NodeType newNodeType = utils->getNodeType(val);
        if (newNodeType == NodeType::SIMPLE) {
            arr->emplace_back(utils->getNodeInternal(val));
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrNew = putIntoArrayAndGet<ArrayNode>(arr);
            addArrayNodeInternally(arrNew, val);
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNew = putIntoArrayAndGet<ObjectNode>(arr);
            addObjectNodeInternally(objNew, val);
        }
    }
    return true;
}


template <typename T>
bool Manager::validateNodeType(ComplexNodePtr node, ErrorCode errorCode)
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
T* Manager::putIntoObjectAndGet(ObjectNode* obj, uint32_t keyID)
{
    obj->emplace(std::make_pair(keyID, T()));
    return &(std::get<T>(obj->at(keyID).value));
}


template <typename T>
T* Manager::putIntoArrayAndGet(ArrayNode* arr)
{
    arr->emplace_back(T());
    return &(std::get<T>(arr->back().value));
}

ArrayNode* Manager::getArrayAndCheckIndex(const std::vector<Path>& path, size_t index)
{
    ComplexNodePtr node = getNodeFromPath(path);
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

std::tuple<ObjectNode*, size_t> Manager::getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr)
{
    ComplexNodePtr node = getNodeFromPath(path);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return { nullptr, 0 };
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);

    std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);
    if (keyID == std::nullopt) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
        return { nullptr, 0 };
    }

    if (obj->contains(keyID.value()) == false) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_INTERNAL_MAP);
        return { nullptr, 0 };
    }
    return { obj, keyID.value() };
}

void Manager::traverseObjectToRemoveKeyID(const ObjectNode& obj)
{
    for (const auto& [keyID, data] : obj) {
        NodeType nodeType = utils->getNodeInternalType(data);
        if (nodeType == NodeType::OBJECT) {
            const auto& objToRemove = std::get<ObjectNode>(data.value);
            traverseObjectToRemoveKeyID(objToRemove);
        }
        else if (nodeType == NodeType::ARRAY) {
            const auto& arrToRemove = std::get<ArrayNode>(data.value);
            traverseArrayToRemoveKeyID(arrToRemove);
        }
        keyMapper->removeKey(keyID);
    }
}


void Manager::traverseArrayToRemoveKeyID(const ArrayNode& arr)
{
    for (const auto& data : arr) {
        NodeType nodeType = utils->getNodeInternalType(data);
        if (nodeType == NodeType::OBJECT) {
            const auto& objToRemove = std::get<ObjectNode>(data.value);
            traverseObjectToRemoveKeyID(objToRemove);
        }
        else if (nodeType == NodeType::ARRAY) {
            const auto& arrToRemove = std::get<ArrayNode>(data.value);
            traverseArrayToRemoveKeyID(arrToRemove);
        }
    }
}
