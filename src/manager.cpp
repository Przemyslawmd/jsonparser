
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
    addObjectInternally(root.get(), node);
    return true;
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
    ObjectNode* obj = checkComplexNode<ObjectNode*>(comNode);
    if (obj == nullptr) {
        return false;
    }

    auto optKeyID = keyMapper->createAndPutKeyID(keyStr, obj->begin()->first);
    if (optKeyID == std::nullopt) {
        return false;
    }
    uint32_t keyID = optKeyID.value();

    NodeType newNodeType = getNodeApiType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        obj->emplace(keyID, createNode(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        ObjectNode* objectNodeNew = putIntoObjectAndGet<ObjectNode>(obj, keyID);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        ArrayNode* arrayNodeNew = putIntoObjectAndGet<ArrayNode>(obj, keyID);
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
    ArrayNode* arr = checkComplexNode<ArrayNode*>(comNode);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeApiType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(createNode(newNode));
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


bool Manager::insertNodeIntoArray(const std::vector<Path>& path, size_t index, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayFromPathAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeApiType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, createNode(newNode));
        return true;
    }
    else if (newNodeType == NodeType::OBJECT) {
        arr->insert(arr->begin() + index, Node{ .value = ObjectNode() });
        ObjectNode* objToAdd = &std::get<ObjectNode>(arr->at(index).value);
        addObjectInternally(objToAdd, newNode);
    }
    else {
        arr->insert(arr->begin() + index, Node{ .value = ArrayNode() });
        ArrayNode* arrToAdd = &std::get<ArrayNode>(arr->at(index).value);
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
        ObjectNode* objNew = &(std::get<ObjectNode>(obj->at(keyID).value));
        addObjectInternally(objNew, newNode);
    }
    else if (nodeType == NodeType::ARRAY) {
        obj->emplace(keyID, ArrayNode());
        ArrayNode* arrNew = &(std::get<ArrayNode>(obj->at(keyID).value));
        addArrayInternally(arrNew, newNode);
    }
    return true;
}


bool Manager::changeNodeInArray(const std::vector<Path>& path, size_t index, const NodeApi& newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayFromPathAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeApiType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arr->at(index) = createNode(newNode);
    }
    else if (nodeType == NodeType::OBJECT) {
        arr->at(index) = Node{ .value = ObjectNode() };
        ObjectNode* objectNodeNew = &std::get<ObjectNode>(arr->at(index).value);
        addObjectInternally(objectNodeNew, newNode);
    }
    else {
        arr->at(index) = Node{ .value = ArrayNode() };
        ArrayNode* arrNodeNew = &std::get<ArrayNode>(arr->at(index).value);
        addArrayInternally(arrNodeNew, newNode);
    }
    return true;
}


bool Manager::removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectFromPathAndKeyID(path, keyStr);
    if (obj == nullptr) {
        return false;
    }

    Node nodeToRemove = obj->at(keyID);
    NodeType nodeType = getNodeType(nodeToRemove);

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

    ArrayNode* arr = getArrayFromPathAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    Node nodeToRemove = arr->at(index);
    NodeType nodeType = getNodeType(nodeToRemove);

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
    ObjectNode* obj = root.get();
    ArrayNode* arr = nullptr;

    for (const auto& pathKey : path) {
        if (nodeType == NodeType::OBJECT && std::holds_alternative<std::string>(pathKey)) {
            const auto& keyStr = std::get<std::string>(pathKey);
            std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);

            if (keyID == std::nullopt) {
                ErrorStorage::putError(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
                return nullptr;
            }

            Node* node = &obj->at(keyID.value());
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodeType = NodeType::OBJECT;
                obj = std::get_if<ObjectNode>(&node->value);
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodeType = NodeType::ARRAY;
                arr = std::get_if<ArrayNode>(&node->value);
            } else {
                ErrorStorage::putError(ErrorCode::MANAGER_IMPROPER_PATH);
                return nullptr;
            }
        }
        else if (nodeType == NodeType::ARRAY && std::holds_alternative<size_t>(pathKey)) {
            size_t index = std::get<size_t>(pathKey);
            if (index >= arr->size()) {
                ErrorStorage::putError(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            Node* node = &arr->at(index);
            if (std::holds_alternative<ObjectNode>(node->value)) {
                nodeType = NodeType::OBJECT;
                obj = std::get_if<ObjectNode>(&node->value);
            }
            else if (std::holds_alternative<ArrayNode>(node->value)) {
                nodeType = NodeType::ARRAY;
                arr = std::get_if<ArrayNode>(&node->value);
            }
            else {
                ErrorStorage::putError(ErrorCode::MANAGER_IMPROPER_PATH);
                return nullptr;
            }
        }
        else {
            ErrorStorage::putError(ErrorCode::MANAGER_IMPROPER_PATH);
            return nullptr;
        }
    }
    if (nodeType == NodeType::OBJECT) {
        return obj;
    }
    return arr;
}


void Manager::addObjectInternally(ObjectNode* obj, const NodeApi& newNode)
{
    uint32_t mapID = keyMapper->getNextMapID();

    for (const auto& [keyStr, val] : std::get<ObjectNodeApi>(newNode.value)) {
        auto optKeyID = keyMapper->createAndPutKeyID(keyStr, mapID);
        uint32_t keyID = optKeyID.value();

        NodeType newNodeType = getNodeApiType(val);
        if (newNodeType == NodeType::SIMPLE) {
            obj->emplace(keyID, createNode(val));
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNew = putIntoObjectAndGet<ObjectNode>(obj, keyID);
            addObjectInternally(objNew, val);
        }
        else {
            ArrayNode* arrNodeNew = putIntoObjectAndGet<ArrayNode>(obj, keyID);
            addArrayInternally(arrNodeNew, val);
        }
    }
}


void Manager::addArrayInternally(ArrayNode* arr, const NodeApi& newNode)
{
    for (auto& val : std::get<ArrayNodeApi>(newNode.value)) {
        NodeType newNodeType = getNodeApiType(val);
        if (newNodeType == NodeType::SIMPLE) {
            arr->emplace_back(createNode(val));
        }
        else if (newNodeType == NodeType::ARRAY) {
            ArrayNode* arrNew = putIntoArrayAndGet<ArrayNode>(arr);
            addArrayInternally(arrNew, val);
        }
        else if (newNodeType == NodeType::OBJECT) {
            ObjectNode* objNew = putIntoArrayAndGet<ObjectNode>(arr);
            addObjectInternally(objNew, val);
        }
    }
}


ArrayNode* 
Manager::getArrayFromPathAndCheckIndex(const std::vector<Path>& path, size_t index)
{
    ComplexNode complexNode = getNodeFromPath(path);
    ArrayNode* arr = checkComplexNode<ArrayNode*>(complexNode);
    if (arr == nullptr) {
        return nullptr;
    }

    if (index > arr->size()) {
        ErrorStorage::putError(ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
        return nullptr;
    }
    return arr;
}


std::tuple<ObjectNode*, size_t> 
Manager::getObjectFromPathAndKeyID(const std::vector<Path>& path, const std::string& keyStr)
{
    ComplexNode comNode = getNodeFromPath(path);
    ObjectNode* obj = checkComplexNode<ObjectNode*>(comNode);
    if (obj == nullptr) {
        return { nullptr, 0 };
    }

    std::optional<uint32_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);
    if (keyID == std::nullopt) {
        ErrorStorage::putError(ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
        return { nullptr, 0 };
    }

    if (obj->contains(keyID.value()) == false) {
        ErrorStorage::putError(ErrorCode::EXCEPTION_MANAGER_NOT_KEY_IN_OBJECT);
        return { nullptr, 0 };
    }
    return { obj, keyID.value() };
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

