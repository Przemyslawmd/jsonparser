
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
    return { writer->createJsonString(root.get()) };
}

/*
bool JsonApi::loadObjectNode(std::unique_ptr<ObjectNode> ObjectNode)
{
    if (root != nullptr) {
        error = std::make_unique<Error>(ErrorCode::API_NOT_EMPTY);
        return false;
    }
    root = std::move(ObjectNode);
    return true;
}


void JsonApi::clear()
{
    root.reset();
    error.reset();
}
*/

/**********************************************************************************/
/* ADD TO OBJECT ******************************************************************/

bool JsonApi::addNodeIntoObject(const std::vector<Indicator>& path, const std::string& key, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    InnerNodePtr objNode = getNode(path);
    if (validateNodeType<ObjectNode*>(objNode, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return false;
    }

    ObjectNode* obj = std::get<ObjectNode*>(objNode);
    size_t newId = keyMapper->putKeyIntoMapAndReturnKeyID(key, obj->begin()->first);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        obj->emplace(std::make_pair(newId, getInternalNode(newNode)));
        return true;
    }
    else if (newNodeType == NodeType::OBJECT) {
        obj->emplace(std::make_pair(newId, ObjectNode()));
        ObjectNode* objNew = &(std::get<ObjectNode>(obj->at(newId).value));
        addObjectNodeInternally(objNew, newNode);
    }
    else if (newNodeType == NodeType::ARRAY) {
        obj->emplace(std::make_pair(newId, ArrayNode()));
        ArrayNode* arrNew = &(std::get<ArrayNode>(obj->at(newId).value));
        addArrayNodeInternally(arrNew, newNode);
    }
    return true;
}


bool JsonApi::addNodeIntoArray(const std::vector<Indicator>& keys, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    InnerNodePtr node = getNode(keys);
    if (validateNodeType<ArrayNode*>(node, ErrorCode::API_NODE_NOT_ARRAY) == false) {
        return false;
    }

    ArrayNode* arr = std::get<ArrayNode*>(node);
    NodeType newNodeType = getNodeType(newNode);

    if (newNodeType == NodeType::SIMPLE) {
        arr->emplace_back(getInternalNode(newNode));
    }
    else if (newNodeType == NodeType::OBJECT) {
        arr->emplace_back(ObjectNode());
        ObjectNode* objNew = &(std::get<ObjectNode>(arr->back().value));
        addObjectNodeInternally(objNew, newNode);
    }
    else if (newNodeType == NodeType::ARRAY) {
        arr->emplace_back(ArrayNode());
        ArrayNode* arrNew = &(std::get<ArrayNode>(arr->back().value));
        addArrayNodeInternally(arrNew, newNode);
    }
    return true;
}


bool JsonApi::insertNodeIntoArray(const std::vector<Indicator>& path, int index, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayNodeAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType newNodeType = getNodeType(newNode);
    if (newNodeType == NodeType::SIMPLE) {
        arr->insert(arr->begin() + index, getInternalNode(newNode));
        return true;
    }
    if (newNodeType == NodeType::OBJECT) {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ObjectNode() });
        ObjectNode* objNode = &std::get<ObjectNode>(arr->at(index).value);
        addObjectNodeInternally(objNode, newNode);
        return true;
    }
    if (newNodeType == NodeType::ARRAY) {
        arr->insert(arr->begin() + index, NodeInternal{ .value = ArrayNode() });
        ArrayNode* arrNode = &std::get<ArrayNode>(arr->at(index).value);
        addArrayNodeInternally(arrNode, newNode);
        return true;
    }
}


bool JsonApi::changeNodeInObject(const std::vector<Indicator>& path, const std::string& key, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    auto [obj, keyID] = getObjectNodeAndCheckKey(path, key);
    if (obj == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        obj->at(keyID) = getInternalNode(newNode);
        return true;
    }

    obj->erase(keyID);
    if (nodeType == NodeType::OBJECT) {
        obj->insert(std::make_pair(keyID, ObjectNode()));
        ObjectNode* objectNew = &(std::get<ObjectNode>(obj->at(keyID).value));
        addObjectNodeInternally(objectNew, newNode);
    }
    else if (nodeType == NodeType::ARRAY) {
        obj->insert(std::make_pair(keyID, ArrayNode()));
        ArrayNode* arrayNew = &(std::get<ArrayNode>(obj->at(keyID).value));
        addArrayNodeInternally(arrayNew, newNode);
    }
    return true;
}


bool JsonApi::changeNodeInArray(const std::vector<Indicator>& path, size_t index, Node newNode)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayNodeAndCheckIndex(path, index);
    if (arr == nullptr) {
        return false;
    }

    NodeType nodeType = getNodeType(newNode);
    if (nodeType == NodeType::SIMPLE) {
        arr->at(index) = getInternalNode(newNode);
        return true;
    }
    // TODO : ObjectNode and ArrayNode

    return true;
}

/**********************************************************************************/
/* PRIVATE ************************************************************************/

bool JsonApi::addObjectNodeInternally(ObjectNode* currentObjectNode, Node newNode)
{
    size_t mapID = keyMapper->getMaxMapID() + (1 << 16);
    size_t nodeID = 0;
    const size_t BIT_MASK = 0b11111111111111110000000000000000;
    for (auto& [key, val] : std::get<ObjectNodeExternal>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        size_t itemID = (mapID & BIT_MASK) + nodeID;
        keyMapper->putKey(key, itemID);
        if (newNodeType == NodeType::SIMPLE) {
            currentObjectNode->emplace(std::make_pair(itemID, getInternalNode(val)));
        }
        else if (newNodeType == NodeType::OBJECT) {
            currentObjectNode->emplace(std::make_pair(itemID, ObjectNode()));
            auto* objNew = &(std::get<ObjectNode>(currentObjectNode->at(itemID).value));
            addObjectNodeInternally(objNew, val);
        }
        nodeID++;
    }
    return true;
}


bool JsonApi::addArrayNodeInternally(ArrayNode* currentObjectNode, Node newNode)
{
    for (auto& val : std::get<ArrayNodeExternal>(newNode.value)) {
        NodeType newNodeType = getNodeType(val);
        if (newNodeType == NodeType::SIMPLE) {
            currentObjectNode->emplace_back(getInternalNode(val));
        }
        else if (newNodeType == NodeType::ARRAY) {
            currentObjectNode->emplace_back(ArrayNode());
            ArrayNode* arrNew = &(std::get<ArrayNode>(currentObjectNode->back().value));
            addArrayNodeInternally(arrNew, val);
        }
        else if (newNodeType == NodeType::OBJECT) {
            currentObjectNode->emplace_back(ObjectNode());
            ObjectNode* objNew = &(std::get<ObjectNode>(currentObjectNode->back().value));
            addObjectNodeInternally(objNew, val);
        }
    }
    return true;
}




/*
bool JsonApi::removeNodeFromObjectNode(const std::vector<Indicator>& keys, const std::string& key)
{
    if (isRootEmpty()) {
        return false;
    }

    ObjectNode* obj = getObjectNodeAndCheckKey(keys, key);
    if (obj == nullptr) {
        return false;
    }

    obj->erase(key);
    return true;
}


bool JsonApi::removeNodeFromArrayNode(const std::vector<Indicator>& keys, size_t index)
{
    if (isRootEmpty()) {
        return false;
    }

    ArrayNode* arr = getArrayNodeAndCheckIndex(keys, index);
    if (arr == nullptr) {
        return false;
    }

    arr->erase(arr->begin() + index);
    return true;
}
*/

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


InnerNodePtr JsonApi::getNode(const std::vector<Indicator>& path)
{
    if (isRootEmpty()) {
        return nullptr;
    }

    InnerNodePtr nodePtr = root.get();
    InnerNodeType lastType = InnerNodeType::OBJECT;

    const auto getNextNode = [](InnerNodePtr* nodePtr, NodeInternal* node, InnerNodeType& lastType) 
    {
        if (std::holds_alternative<ObjectNode>(node->value)) {
            *nodePtr = std::get_if<ObjectNode>(&node->value);
            lastType = InnerNodeType::OBJECT;
        }
        else if (std::holds_alternative<ArrayNode>(node->value)) {
            *nodePtr = std::get_if<ArrayNode>(&node->value);
            lastType = InnerNodeType::ARRAY;
        }
    };

    for (const auto indicator : path) {
        if (lastType == InnerNodeType::OBJECT && std::holds_alternative<std::string>(indicator)) {
            ObjectNode* obj = std::get<ObjectNode*>(nodePtr);
            const auto& keyStr = std::get<std::string>(indicator);
            std::optional<size_t> keyID = keyMapper->getKeyID(keyStr, obj->begin()->first);
            if (keyID == std::nullopt) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }
            if (obj->contains(keyID.value()) == false) {
                error = std::make_unique<Error>(ErrorCode::API_NOT_KEY_IN_MAP);
                return nullptr;
            }

            NodeInternal* node = &obj->at(keyID.value());
            getNextNode(&nodePtr, node, lastType);
        }
        else if (lastType == InnerNodeType::ARRAY && std::holds_alternative<size_t>(indicator)) {
            ArrayNode* arr = std::get<ArrayNode*>(nodePtr);
            size_t index = std::get<size_t>(indicator);
            if (index >= arr->size()) {
                error = std::make_unique<Error>(ErrorCode::API_INDEX_OUT_OF_ARRAY);
                return nullptr;
            }

            NodeInternal* node = &arr->at(index);
            getNextNode(&nodePtr, node, lastType);
        }
        else {
            error = std::make_unique<Error>(ErrorCode::API_INCONSISTENT_DATA);
            return nullptr;
        }
    }
    return nodePtr;
}


template <typename T>
bool JsonApi::validateNodeType(InnerNodePtr node, ErrorCode potentialError)
{
    if (std::holds_alternative<nullptr_t>(node)) {
        return false;
    }
    if (std::holds_alternative<T>(node) == false) {
        error = std::make_unique<Error>(potentialError);
        return false;
    }
    return true;
}


ArrayNode* JsonApi::getArrayNodeAndCheckIndex(const std::vector<Indicator>& path, size_t index)
{
    InnerNodePtr node = getNode(path);
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


std::tuple<ObjectNode*, size_t> JsonApi::getObjectNodeAndCheckKey(const std::vector<Indicator>& path, const std::string& key)
{
    InnerNodePtr node = getNode(path);
    if (validateNodeType<ObjectNode*>(node, ErrorCode::API_NODE_NOT_OBJECT) == false) {
        return { nullptr, 0 };
    }

    ObjectNode* obj = std::get<ObjectNode*>(node);

    std::optional<size_t> keyID = keyMapper->getKeyID(key, obj->begin()->first);
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


NodeType JsonApi::getNodeType(Node& node)
{
    if (std::holds_alternative<ObjectNodeExternal>(node.value)) {
        return NodeType::OBJECT;
    }
    if (std::holds_alternative<ArrayNodeExternal>(node.value)) {
        return NodeType::ARRAY;
    }
    return NodeType::SIMPLE;
}


NodeInternal JsonApi::getInternalNode(Node& node)
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
    // TODO : Error if change not for simple value 
}
