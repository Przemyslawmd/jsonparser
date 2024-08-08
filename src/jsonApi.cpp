
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


std::string JsonApi::parseObjectNodeToJsonString()
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


bool JsonApi::loadObjectJson(const Node& node)
{
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_EMPTY);
        return false;
    }
    root = std::make_unique<ObjectNode>();
    addObjectNodeInternally(root.get(), node);
    return true;
}


void JsonApi::clear()
{
    root.reset();
    error.reset();
    keyMapper->clear();
}

/**********************************************************************************/
/* ADD TO OBJECT ******************************************************************/

bool JsonApi::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr objNode = getNodeFromPath(path);
    if (validateNodeType<ObjectNode*>(objNode, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return false;
    }

    ObjectNode* obj = std::get<ObjectNode*>(objNode);
    uint32_t newID = keyMapper->putKeyIntoMapAndReturnKeyID(keyStr, obj->begin()->first);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        obj->emplace(std::make_pair(newID, getNodeInternal(newNode)));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objNew = putIntoObjectAndGet<ObjectNode>(obj, newID);
        addObjectNodeInternally(objNew, newNode);
    }
    else if (newNodeType == NodeType::ARRAY) {
        ArrayNode* arrNew = putIntoObjectAndGet<ArrayNode>(obj, newID);
        addArrayNodeInternally(arrNew, newNode);
    }
    return true;
}


bool JsonApi::addNodeIntoArray(const std::vector<Path>& path, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr node = getNodeFromPath(path);
    if (validateNodeType<ArrayNode*>(node, ErrorCode::API_NODE_NOT_ARRAY) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(getNodeInternal(newNode));
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


bool JsonApi::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, getNodeInternal(newNode));
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


bool JsonApi::changeNodeInObject(const std::vector<Path>& path, const std::string& key, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectAndKeyID(path, key);
    if (obj == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        obj->at(keyID) = getNodeInternal(newNode);
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


bool JsonApi::changeNodeInArray(const std::vector<Path>& path, size_t index, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arr->at(index) = getNodeInternal(newNode);
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


bool JsonApi::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectAndKeyID(path, keyStr);
    if (obj == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = obj->at(keyID);
    NodeType nodeType = getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        ObjectNode* objToRemove = &(std::get<ObjectNode>(obj->at(keyID).value));
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        ArrayNode* arrToRemove = &(std::get<ArrayNode>(obj->at(keyID).value));
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    obj->erase(keyID);
    keyMapper->removeKey(keyID);
    return true;
}


bool JsonApi::removeNodeFromArray(const std::vector<Path>& path, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = arr->at(index);
    NodeType nodeType = getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        ObjectNode* objToRemove = &(std::get<ObjectNode>(arr->at(index).value));
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        ArrayNode* arrToRemove = &(std::get<ArrayNode>(arr->at(index).value));
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    arr->erase(arr->begin() + index);
    return true;
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

bool JsonApi::isRootEmpty()
{
    if (root == nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_EMPTY);
        return true;
    }
    return false;
}


ComplexNodePtr JsonApi::getNodeFromPath(const std::vector<Path>& path)
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
            nodeComplexPtr = getNextNode( node);
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


bool JsonApi::addObjectNodeInternally(ObjectNode* obj, const Node& newNode)
{
    uint32_t mapID = keyMapper->getNextMapID();
    uint32_t nodeID = 0;

    for (auto& [key, val] : std::get<ObjectNodeApi>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        uint32_t itemID = keyMapper->createItemID(mapID, nodeID);
        keyMapper->putKey(key, itemID);

        if (newNodeType == NodeType::SIMPLE) {
            obj->emplace(std::make_pair(itemID, getNodeInternal(val)));
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNew = putIntoObjectAndGet<ObjectNode>(obj, itemID);
            addObjectNodeInternally(objNew, val);
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrNew = putIntoObjectAndGet<ArrayNode>(obj, itemID);
            addArrayNodeInternally(arrNew, val);
        }
        nodeID++;
    }
    return true;
}


bool JsonApi::addArrayNodeInternally(ArrayNode* arr, const Node& newNode)
{
    for (auto& val : std::get<ArrayNodeApi>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        if (newNodeType == NodeType::SIMPLE) {
            arr->emplace_back(getNodeInternal(val));
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


ArrayNode* JsonApi::getArrayAndCheckIndex(const std::vector<Path>& path, size_t index)
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


std::tuple<ObjectNode*, size_t> JsonApi::getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr)
{
    ComplexNodePtr node = getNodeFromPath(path);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return { nullptr, 0 };
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);

    std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);
    if (keyID == std::nullopt) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
        return { nullptr, 0 } ;
    }

    if (obj->contains(keyID.value()) == false) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_INTERNAL_MAP);
        return { nullptr, 0 };
    }
    return { obj, keyID.value() };
}


template <typename T>
T* JsonApi::putIntoObjectAndGet(ObjectNode* obj, uint32_t itemID)
{
    obj->emplace(std::make_pair(itemID, T()));
    return &(std::get<T>(obj->at(itemID).value));
}


template <typename T>
T* JsonApi::putIntoArrayAndGet(ArrayNode* arr)
{
    arr->emplace_back(T());
    return &(std::get<T>(arr->back().value));
}


bool JsonApi::traverseObjectToRemoveKeyID(ObjectNode* currentObject)
{
    for (auto& [keyID, data] : *currentObject) {
        NodeType nodeType = getNodeInternalType(data);
        if (nodeType == NodeType::OBJECT) {
            ObjectNode* objectToRemove = &std::get<ObjectNode>(data.value);
            traverseObjectToRemoveKeyID(objectToRemove);
        }
        else if (nodeType == NodeType::ARRAY) {
            ArrayNode* arrayToRemove = &std::get<ArrayNode>(data.value);
            traverseArrayToRemoveKeyID(arrayToRemove);
        }
        keyMapper->removeKey(keyID);
    }
    return true;
}


bool JsonApi::traverseArrayToRemoveKeyID(ArrayNode* currentArray)
{
    for (auto& data : *currentArray) {
        NodeType nodeType = getNodeInternalType(data);
        if (nodeType == NodeType::OBJECT) {
            ObjectNode* objectToRemove = &std::get<ObjectNode>(data.value);
            traverseObjectToRemoveKeyID(objectToRemove);
        }
        else if (nodeType == NodeType::ARRAY) {
            ArrayNode* arrayToRemove = &std::get<ArrayNode>(data.value);
            traverseArrayToRemoveKeyID(arrayToRemove);
        }
    }
    return true;
}


NodeType JsonApi::getNodeType(const Node& node)
{
    if (std::holds_alternative<ObjectNodeApi>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNodeApi>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


NodeType JsonApi::getNodeInternalType(const NodeInternal& node)
{
    if (std::holds_alternative<ObjectNode>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNode>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


NodeInternal JsonApi::getNodeInternal(const Node& node)
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
    if (std::holds_alternative<nullptr_t>(node.value)) {
        return NodeInternal{ .value = nullptr };
    }
    // TODO : Error if change not for simple value 
}
