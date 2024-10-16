
#include "manager.h"

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"
#include "../src/log/ErrorStorage.h"
#include "utils.h"


using ObjectNodeApi = std::map<std::string, NodeApi>;
using ArrayNodeApi = std::vector<NodeApi>;


Manager::Manager()
{
    keyMapper = std::make_unique<KeyMapper>();
    ErrorStorage::clear();
}


void Manager::clear()
{
    root.reset();
    keyMapper->clear();
    ErrorStorage::clear();
}


bool Manager::parseJsonString(const std::string& jsonString)
{
    ErrorStorage::clear();
    if (root != nullptr) {
        ErrorStorage::putError(ErrorCode::MANAGER_ROOT_NOT_EMPTY);
        return false;
    }

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    if (tokens == nullptr) {
        return false;
    }

    const auto validator = std::make_unique<Validator>();
    if (validator->validate(*tokens) == false) {
        return false;
    }

    tokens = createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>(*keyMapper);
    root = parser->parseTokens(*tokens);
    if (root == nullptr) {
        return false;
    }
    return true;
}


std::optional<std::string> Manager::parseObjectToString()
{
    if (isRootEmpty()) {
        return std::nullopt;
    }
    const auto writer = std::make_unique<Writer>(*keyMapper);
    return writer->createJsonString(*root);
}


bool Manager::loadJsonObject(const NodeApi& node)
{
    if (root != nullptr) {
        ErrorStorage::putError(ErrorCode::MANAGER_ROOT_NOT_EMPTY);
        return false;
    }

    if (std::holds_alternative<std::map<std::string, NodeApi>>(node.value) == false) {
        ErrorStorage::putError(ErrorCode::MANAGER_ROOT_NOT_OBJECT);
        return false;
    }

    root = std::make_unique<ObjectNode>();
    return addObjectInternally(root.get(), node);
}


bool Manager::isJsonObject() const
{
    return isRootEmpty() == false;
}


bool Manager::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNode comNode = getNodeFromPath(path);
    ObjectNode* objNode = checkComplexNode<ObjectNode*>(comNode);
    if (objNode == nullptr) {
        return false;
    }

    auto optKeyID = keyMapper->createAndPutKeyID(keyStr, objNode->begin()->first);
    if (optKeyID == std::nullopt) {
        return false;
    }
    uint32_t keyID = optKeyID.value();

    NodeType newNodeType = getNodeApiType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        objNode->emplace(keyID, createNode(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objectNodeNew = putIntoObjectAndGet<ObjectNode>(objNode, keyID);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        ArrayNode* arrayNodeNew = putIntoObjectAndGet<ArrayNode>(objNode, keyID);
        addArrayInternally(arrayNodeNew, newNode);
    }
    return true;
}


bool Manager::addNodeIntoArray(const std::vector<Path>& path, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNode comNode = getNodeFromPath(path);
    ArrayNode* arrNode = checkComplexNode<ArrayNode*>(comNode);
    if (arrNode == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeApiType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arrNode->emplace_back(createNode(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objNew = putIntoArrayAndGet<ObjectNode>(arrNode);
        addObjectInternally(objNew, newNode);
    }
    else {
        ArrayNode* arrNew = putIntoArrayAndGet<ArrayNode>(arrNode);
        addArrayInternally(arrNew, newNode);
    }
    return true;
}


bool Manager::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arrNode = getArrayFromPathAndCheckIndex(path, index);
    if (arrNode == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeApiType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arrNode->insert(arrNode->begin() + index, createNode(newNode));
        return true;
    }
    else if (newNodeType == NodeType::OBJECT) {
        arrNode->insert(arrNode->begin() + index, Node{ .value = ObjectNode() });
        ObjectNode* objToAdd = &std::get<ObjectNode>(arrNode->at(index).value);
        addObjectInternally(objToAdd, newNode);
        return true;
    }
    else {
        arrNode->insert(arrNode->begin() + index, Node{ .value = ArrayNode() });
        ArrayNode* arrToAdd = &std::get<ArrayNode>(arrNode->at(index).value);
        addArrayInternally(arrToAdd, newNode);
    }
    return true;
}


bool Manager::changeNodeInObject(const std::vector<Path>& path, const std::string& keyStr, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectFromPathAndKeyID(path, keyStr);
    if (obj == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeApiType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        obj->at(keyID) = createNode(newNode);
        return true;
    }

    obj->erase(keyID);
    if (nodeType == NodeType::OBJECT) {
        obj->emplace(keyID, ObjectNode());
        ObjectNode* objToAdd = &(std::get<ObjectNode>(obj->at(keyID).value));
        addObjectInternally(objToAdd, newNode);
    }
    else if (nodeType == NodeType::ARRAY) {
        obj->emplace(keyID, ArrayNode());
        ArrayNode* arrToAdd = &(std::get<ArrayNode>(obj->at(keyID).value));
        addArrayInternally(arrToAdd, newNode);
    }
    return true;
}


bool Manager::changeNodeInArray(const std::vector<Path>& path, size_t index, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arrNode = getArrayFromPathAndCheckIndex(path, index);
    if (arrNode == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeApiType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arrNode->at(index) = createNode(newNode);
    }
    else if (nodeType == NodeType::OBJECT) {
        arrNode->at(index) = Node{ .value = ObjectNode() };
        ObjectNode* objectNodeNew = &std::get<ObjectNode>(arrNode->at(index).value);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        arrNode->at(index) = Node{ .value = ArrayNode() };
        ArrayNode* arrayNodeNew = &std::get<ArrayNode>(arrNode->at(index).value);
        addArrayInternally(arrayNodeNew, newNode);
    }
    return true;
}


bool Manager::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [objNode, keyID] = getObjectFromPathAndKeyID(path, keyStr);
    if (objNode == nullptr) {
        return false;
    }

    Node nodeToRemove = objNode->at(keyID);
    NodeType nodeType = getNodeType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(objNode->at(keyID).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(objNode->at(keyID).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    objNode->erase(keyID);
    keyMapper->removeKey(keyID);
    return true;
}


bool Manager::removeNodeFromArray(const std::vector<Path>& path, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arrNode = getArrayFromPathAndCheckIndex(path, index);
    if (arrNode == nullptr) {
        return false;
    }

    Node nodeToRemove = arrNode->at(index);
    NodeType nodeType = getNodeType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(arrNode->at(index).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(arrNode->at(index).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    arrNode->erase(arrNode->begin() + index);
    return true;
}


const std::vector<Error>& Manager::getErrors() const
{
    return ErrorStorage::getErrors();
}

/*******************************************************************/
/* PRIVATE *********************************************************/

bool Manager::isRootEmpty() const
{
    if (root == nullptr) {
        ErrorStorage::putError(ErrorCode::MANAGER_EMPTY);
        return true;
    }
    return false;
}


ComplexNode Manager::getNodeFromPath(const std::vector<Path>& path)
{
    if (path.empty()) {
        return root.get();
    }

    NodeType nodeType = NodeType::OBJECT;
    ObjectNode* objNode = root.get();
    ArrayNode* arrNode = nullptr;

    for (const auto& pathKey : path) {
        if (nodeType == NodeType::OBJECT && std::holds_alternative<std::string>(pathKey)) {
            const auto& keyStr = std::get<std::string>(pathKey);
            std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, objNode->begin()->first);

            if (keyID == std::nullopt) {
                ErrorStorage::putError(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
                return nullptr;
            }

            Node* node = &objNode->at(keyID.value());
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodeType = NodeType::OBJECT;
                objNode = std::get_if<ObjectNode>(&node->value);
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodeType = NodeType::ARRAY;
                arrNode = std::get_if<ArrayNode>(&node->value);
            }
        }
        else if (nodeType == NodeType::ARRAY && std::holds_alternative<size_t>(pathKey)) {
            size_t index = std::get<size_t>(pathKey);
            if (index >= arrNode->size()) {
                ErrorStorage::putError(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            Node* node = &arrNode->at(index);
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodeType = NodeType::OBJECT;
                objNode = std::get_if<ObjectNode>(&node->value);
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodeType = NodeType::ARRAY;
                arrNode = std::get_if<ArrayNode>(&node->value);
            }
        }
        else {
            ErrorStorage::putError(ErrorCode::MANAGER_IMPROPER_PATH);
            return nullptr;
        }
    }
    if (nodeType == NodeType::OBJECT) {
        return objNode;
    }
    return arrNode;
}


bool Manager::addObjectInternally(ObjectNode* objectNode, const NodeApi& newNode)
{
    uint32_t mapID = keyMapper->getNextMapID();

    for (const auto& [keyStr, val] : std::get<ObjectNodeApi>(newNode.value)) {
        auto optKeyID = keyMapper->createAndPutKeyID(keyStr, mapID);
        if (optKeyID == std::nullopt) {
            return false;
        }
        uint32_t keyID = optKeyID.value();

        NodeType newNodeType = getNodeApiType(val);
        if (newNodeType == NodeType::SIMPLE) {
            objectNode->emplace(keyID, createNode(val));
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNodeNew = putIntoObjectAndGet<ObjectNode>(objectNode, keyID);
            addObjectInternally(objNodeNew, val);
        }
        else {
            ArrayNode* arrNodeNew = putIntoObjectAndGet<ArrayNode>(objectNode, keyID);
            addArrayInternally(arrNodeNew, val);
        }
    }
    return true;
}


bool Manager::addArrayInternally(ArrayNode* arrNode, const NodeApi& newNode)
{
    for (auto& val : std::get<ArrayNodeApi>(newNode.value)) {
        NodeType newNodeType = getNodeApiType(val);
        if (newNodeType == NodeType::SIMPLE) {
            arrNode->emplace_back(createNode(val));
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrayNodeNew = putIntoArrayAndGet<ArrayNode>(arrNode);
            addArrayInternally(arrayNodeNew, val);
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objectNodeNew = putIntoArrayAndGet<ObjectNode>(arrNode);
            addObjectInternally(objectNodeNew, val);
        }
    }
    return true;
}


template <typename T>
T Manager::checkComplexNode(ComplexNode node)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return nullptr;
    }
    if (std::holds_alternative<T>(node) == false) {
        if (std::is_same<T, ObjectNode*>::value) {
            ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_OBJECT);
        }
        else if (std::is_same<T, ArrayNode*>::value) {
            ErrorStorage::putError(ErrorCode::MANAGER_NODE_NOT_ARRAY);
        }
        return nullptr;
    }
    return std::get<T>(node);
}


template <typename T>
T* Manager::putIntoObjectAndGet(ObjectNode* objectNode, uint32_t keyID)
{
    objectNode->emplace(keyID, T());
    return &(std::get<T>(objectNode->at(keyID).value));
}


template <typename T>
T* Manager::putIntoArrayAndGet(ArrayNode* arrayNode)
{
    arrayNode->emplace_back(T());
    return &(std::get<T>(arrayNode->back().value));
}


ArrayNode* 
Manager::getArrayFromPathAndCheckIndex(const std::vector<Path>& path, size_t index)
{
    ComplexNode complexNode = getNodeFromPath(path);
    ArrayNode* arrayNode = checkComplexNode<ArrayNode*>(complexNode);
    if (arrayNode == nullptr) {
        return nullptr;
    }

    if (index > arrayNode->size()) {
        ErrorStorage::putError(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
        return nullptr;
    }
    return arrayNode;
}


std::tuple<ObjectNode*, size_t> 
Manager::getObjectFromPathAndKeyID(const std::vector<Path>& path, const std::string& keyStr)
{
    ComplexNode complexNode = getNodeFromPath(path);
    ObjectNode* objectNode = checkComplexNode<ObjectNode*>(complexNode);
    if (objectNode == nullptr) {
        return { nullptr, 0 };
    }

    std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, objectNode->begin()->first);
    if (keyID == std::nullopt) {
        ErrorStorage::putError(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
        return { nullptr, 0 };
    }

    if (objectNode->contains(keyID.value()) == false) {
        ErrorStorage::putError(ErrorCode::MANAGER_NOT_KEY_IN_INTERNAL_OBJECT);
        return { nullptr, 0 };
    }
    return { objectNode, keyID.value() };
}


void Manager::traverseObjectToRemoveKeyID(const ObjectNode& obj)
{
    for (const auto& [keyID, data] : obj) {
        NodeType nodeType = getNodeType(data);
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
        NodeType nodeType = getNodeType(data);
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

