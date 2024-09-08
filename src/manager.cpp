
#include "manager.h"

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"
#include "utils.h"


using ObjectNodeApi = std::map<std::string, Node>;
using ArrayNodeApi = std::vector<Node>;


Manager::Manager()
{
    keyMapper = std::make_unique<KeyMapper>();
}


void Manager::clear()
{
    root.reset();
    error.reset();
    keyMapper->clear();
}


bool Manager::parseJsonString(const std::string& jsonString)
{
    error.reset();
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::MANAGER_ROOT_NOT_EMPTY);
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

    const auto parser = std::make_unique<Parser>(*keyMapper);
    root = parser->parseTokens(*tokens);
    if (root == nullptr) {
        error = parser->getError();
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
    auto jsonStr = writer->createJsonString(*root);
    if (jsonStr == std::nullopt) {
        error = writer->getError();
    }
    return jsonStr;
}


bool Manager::loadJsonObject(const Node& node)
{
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::MANAGER_ROOT_NOT_EMPTY);
        return false;
    }
    root = std::make_unique<ObjectNode>();
    return addObjectInternally(root.get(), node);
}


bool Manager::addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr objNode = getNodeFromPath(path);
    if (validateComplexNode<ObjectNode*>(objNode) == false) {
        return false;
    }

    ObjectNode* objectNode = std::get<ObjectNode*>(objNode);

    auto optKeyID = keyMapper->createAndPutKeyID(keyStr, objectNode->begin()->first);
    if (optKeyID == std::nullopt) {
        return false;
    }
    uint32_t keyID = optKeyID.value();

    NodeType newNodeType = getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        objectNode->emplace(std::make_pair(keyID, getNodeInternal(newNode)));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objectNodeNew = putIntoObjectAndGet<ObjectNode>(objectNode, keyID);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        ArrayNode* arrayNodeNew = putIntoObjectAndGet<ArrayNode>(objectNode, keyID);
        addArrayInternally(arrayNodeNew, newNode);
    }
    return true;
}


bool Manager::addNodeIntoArray(const std::vector<Path>& path, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ComplexNodePtr node = getNodeFromPath(path);
    if (validateComplexNode<ArrayNode*>(node) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(getNodeInternal(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objNew = putIntoArrayAndGet<ObjectNode>(arr);
        addObjectInternally(objNew, newNode);
    }
    else {
        ArrayNode* arrNew = putIntoArrayAndGet<ArrayNode>(arr);
        addArrayInternally(arrNew, newNode);
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

    NodeType newNodeType = getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, getNodeInternal(newNode));
        return true;
    }
    else if (newNodeType == NodeType::OBJECT) {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ObjectNode() });
        ObjectNode* objToAdd = &std::get<ObjectNode>(arr->at(index).value);
        addObjectInternally(objToAdd, newNode);
        return true;
    }
    else {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ArrayNode() });
        ArrayNode* arrToAdd = &std::get<ArrayNode>(arr->at(index).value);
        addArrayInternally(arrToAdd, newNode);
    }
    return true;
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

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        obj->at(keyID) = getNodeInternal(newNode);
        return true;
    }

    obj->erase(keyID);
    if (nodeType == NodeType::OBJECT) {
        obj->insert(std::make_pair(keyID, ObjectNode()));
        ObjectNode* objToAdd = &(std::get<ObjectNode>(obj->at(keyID).value));
        addObjectInternally(objToAdd, newNode);
    }
    else if (nodeType == NodeType::ARRAY) {
        obj->insert(std::make_pair(keyID, ArrayNode()));
        ArrayNode* arrToAdd = &(std::get<ArrayNode>(obj->at(keyID).value));
        addArrayInternally(arrToAdd, newNode);
    }
    return true;
}


bool Manager::changeNodeInArray(const std::vector<Path>& path, size_t index, const Node& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arrayNode = getArrayAndCheckIndex(path, index);
    if (arrayNode == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arrayNode->at(index) = getNodeInternal(newNode);
    }
    else if (nodeType == NodeType::OBJECT) {
        arrayNode->at(index) = NodeInternal{ .value = ObjectNode() };
        ObjectNode* objectNodeNew = &std::get<ObjectNode>(arrayNode->at(index).value);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        arrayNode->at(index) = NodeInternal{ .value = ArrayNode() };
        ArrayNode* arrayNodeNew = &std::get<ArrayNode>(arrayNode->at(index).value);
        addArrayInternally(arrayNodeNew, newNode);
    }
    return true;
}


bool Manager::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [objectNode, keyID] = getObjectAndKeyID(path, keyStr);
    if (objectNode == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = objectNode->at(keyID);
    NodeType nodeType = getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(objectNode->at(keyID).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(objectNode->at(keyID).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    objectNode->erase(keyID);
    keyMapper->removeKey(keyID);
    return true;
}


bool Manager::removeNodeFromArray(const std::vector<Path>& path, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arrayNode = getArrayAndCheckIndex(path, index);
    if (arrayNode == nullptr) {
        return false;
    }

    NodeInternal nodeToRemove = arrayNode->at(index);
    NodeType nodeType = getNodeInternalType(nodeToRemove);

    if (nodeType == NodeType::OBJECT) {
        const auto& objToRemove = std::get<ObjectNode>(arrayNode->at(index).value);
        traverseObjectToRemoveKeyID(objToRemove);
    }
    else if (nodeType == NodeType::ARRAY) {
        const auto& arrToRemove = std::get<ArrayNode>(arrayNode->at(index).value);
        traverseArrayToRemoveKeyID(arrToRemove);
    }
    arrayNode->erase(arrayNode->begin() + index);
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
        error = std::make_unique<Error>(ErrorCode::MANAGER_EMPTY);
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
                error = std::make_unique<Error>(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
                return nullptr;
            }
            if (obj->contains(keyID.value()) == false) {
                error = std::make_unique<Error>(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
                return nullptr;
            }
            NodeInternal* node = &obj->at(keyID.value());
            nodeComplexPtr = getNextNode(node);
        }
        else if (nodeType == NodeType::ARRAY && std::holds_alternative<size_t>(pathKey)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodeComplexPtr);
            size_t index = std::get<size_t>(pathKey);

            if (index >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }
            NodeInternal* node = &arr->at(index);
            nodeComplexPtr = getNextNode(node);
        }
        else {
            error = std::make_unique<Error>(ErrorCode::MANAGER_IMPROPER_PATH);
            return nullptr;
        }
    }
    return nodeComplexPtr;
}


bool Manager::addObjectInternally(ObjectNode* objectNode, const Node& newNode)
{
    uint32_t mapID = keyMapper->getNextMapID();

    for (const auto& [keyStr, val] : std::get<ObjectNodeApi>(newNode.value)) {
        auto optKeyID = keyMapper->createAndPutKeyID(keyStr, mapID);
        if (optKeyID == std::nullopt) {
            return false;
        }
        uint32_t keyID = optKeyID.value();

        NodeType newNodeType = getNodeType(val);
        if (newNodeType == NodeType::SIMPLE) {
            objectNode->emplace(std::make_pair(keyID, getNodeInternal(val)));
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objectNodeNew = putIntoObjectAndGet<ObjectNode>(objectNode, keyID);
            addObjectInternally(objectNodeNew, val);
        }
        else {
            ArrayNode* arrayNodeNew = putIntoObjectAndGet<ArrayNode>(objectNode, keyID);
            addArrayInternally(arrayNodeNew, val);
        }
    }
    return true;
}


bool Manager::addArrayInternally(ArrayNode* arrayNode, const Node& newNode)
{
    for (auto& val : std::get<ArrayNodeApi>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        if (newNodeType == NodeType::SIMPLE) {
            arrayNode->emplace_back(getNodeInternal(val));
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrayNodeNew = putIntoArrayAndGet<ArrayNode>(arrayNode);
            addArrayInternally(arrayNodeNew, val);
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objectNodeNew = putIntoArrayAndGet<ObjectNode>(arrayNode);
            addObjectInternally(objectNodeNew, val);
        }
    }
    return true;
}


template <typename T>
bool Manager::validateComplexNode(ComplexNodePtr node)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return false;
    }
    if (std::holds_alternative<T>(node) == false) {
        if (std::is_same<T, ObjectNode*>::value) {
            error = std::make_unique<Error>(ErrorCode::MANAGER_NODE_NOT_OBJECT);
        }
        else if (std::is_same<T, ArrayNode*>::value) {
            error = std::make_unique<Error>(ErrorCode::MANAGER_NODE_NOT_ARRAY);
        }
        return false;
    }
    return true;
}


template <typename T>
T* Manager::putIntoObjectAndGet(ObjectNode* objectNode, uint32_t keyID)
{
    objectNode->emplace(std::make_pair(keyID, T()));
    return &(std::get<T>(objectNode->at(keyID).value));
}


template <typename T>
T* Manager::putIntoArrayAndGet(ArrayNode* arrayNode)
{
    arrayNode->emplace_back(T());
    return &(std::get<T>(arrayNode->back().value));
}


ArrayNode* Manager::getArrayAndCheckIndex(const std::vector<Path>& path, size_t index)
{
    ComplexNodePtr node = getNodeFromPath(path);
    if (validateComplexNode<ArrayNode*>(node) == false) {
        return nullptr;
    }

    ArrayNode* arrayNode = std::get<ArrayNode*>(node);
    if (index > arrayNode->size()) {
        error = std::make_unique<Error>(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
        return nullptr;
    }
    return arrayNode;
}


std::tuple<ObjectNode*, size_t> Manager::getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr)
{
    ComplexNodePtr node = getNodeFromPath(path);
    if (validateComplexNode<ObjectNode*>(node) == false) {
        return { nullptr, 0 };
    }

    ObjectNode* objectNode = std::get<ObjectNode*>(node);

    std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, objectNode->begin()->first);
    if (keyID == std::nullopt) {
        error = std::make_unique<Error>(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
        return { nullptr, 0 };
    }

    if (objectNode->contains(keyID.value()) == false) {
        error = std::make_unique<Error>(ErrorCode::MANAGER_NOT_KEY_IN_INTERNAL_OBJECT);
        return { nullptr, 0 };
    }
    return { objectNode, keyID.value() };
}


void Manager::traverseObjectToRemoveKeyID(const ObjectNode& obj)
{
    for (const auto& [keyID, data] : obj) {
        NodeType nodeType = getNodeInternalType(data);
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
        NodeType nodeType = getNodeInternalType(data);
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

