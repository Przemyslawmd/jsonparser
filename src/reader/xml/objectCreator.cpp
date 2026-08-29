
#include "objectCreator.h"

#include <ranges>


using namespace xml;
using enum State;

std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<Elem>& elems)
{
    unsigned int firstTag = 0;
    if (elems.front().type == ElemType::DECLARATION) {
        keyMapper.storeAttrsDec(std::move(elems.front().attr));
        firstTag = 1;
    }

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name.value(), OBJECT_PARSING);
    attrs = &elems.at(firstTag).attr;

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name.value());
                attrs = &elem.attr;
                break;
            case TAG_ARRAY_BEGIN:
                processTagArrayOpen(elem.name.value());
                attrs = &elem.attr;
                break;
            case TAG_CLOSE:
                popContext();
                break;
            case TAG_ARRAY_END:
                nodeStack.pop();
                stateStack.pop();
                keyStack.pop();
                break;
            case CONTENT:
                processContent(elem.value);
                break;
            case TAG_ARRAY_OPEN:
            case TAG_ARRAY_CLOSE:
                break;
        }
    }
    return document;
}


void ObjectCreator::processTagOpen(const std::string& keyStr)
{
    if (stateStack.top() == OBJECT_PARSING)
    {
        ObjectNode* objNode = std::get<ObjectNode*>(nodeStack.top());
        auto optPrevKey = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
        if (optPrevKey.has_value()) {
            ObjectNode* currNode = std::get_if<ObjectNode>(&objNode->at(optPrevKey.value()).value);
            pushContext(currNode, keyStr, OBJECT_PARSING);
            return;
        }

        optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        uint32_t prevKey = optPrevKey.value();
        objNode->emplace(prevKey, ObjectNode());
        ObjectNode* newNode = std::get_if<ObjectNode>(&objNode->at(prevKey).value);

        if (attrs && !attrs->empty()) {
            insertAttrs(*newNode, *attrs);
        }
        pushContext(newNode, keyStr, OBJECT_PARSING);
    }
    else {
        ArrayNode* arrNode = std::get<ArrayNode*>(nodeStack.top());
        Node& ref = arrNode->emplace_back(ObjectNode());
        ObjectNode* newNode = std::get_if<ObjectNode>(&ref.value);
        pushContext(newNode, keyStr, OBJECT_PARSING);
    }
}


void ObjectCreator::processTagArrayOpen(const std::string& keyStr)
{
    ObjectNode* objNode = std::get<ObjectNode*>(nodeStack.top());
    auto prevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    objNode->emplace(prevKey.value(), ObjectNode());
    ObjectNode* currNode = std::get_if<ObjectNode>(&objNode->at(prevKey.value()).value);
    pushContext(currNode, keyStr, OBJECT_PARSING);

    auto arrKey = keyMapper.createKeyID(keyStr, mapIDStack.top());
    currNode->emplace(arrKey.value(), ArrayNode());
    ArrayNode* arrNode = std::get_if<ArrayNode>(&currNode->at(arrKey.value()).value);
    pushContext(arrNode, ARRAY_PARSING);
}


void ObjectCreator::processContent(TokenData& data)
{
    if (stateStack.top() == OBJECT_PARSING)
    {
        auto optKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        if (!optKey.has_value()) {
            return;
        }
        uint32_t keyID = optKey.value();
        ObjectNode* objNode = std::get<ObjectNode*>(nodeStack.top());

        if (attrs->empty()) {
            std::visit([objNode, keyID](auto&& val) { objNode->emplace(keyID, val); }, data);
            return;
        }

        objNode->emplace(keyID, ObjectNode());
        ObjectNode* currNode = std::get_if<ObjectNode>(&objNode->at(keyID).value);
        insertAttrs(*currNode, *attrs);
        auto newKeyID = keyMapper.createKeyID(pretendedKey, mapIDStack.top());
        std::visit([currNode, newKeyID](auto&& val) { currNode->emplace(newKeyID.value(), val); }, data);
    } 
    else {
        ArrayNode* arrNode  = std::get<ArrayNode*>(nodeStack.top());
        std::visit([arrNode](auto&& val) { arrNode->emplace_back(val); }, data);
    }
}


void ObjectCreator::insertAttrs(ObjectNode& node, std::map<std::string, std::string>& attrs)
{
    for (const auto& [keyStr, value] : attrs) {
        auto keyId = keyMapper.createKeyIDAttr(keyStr, mapIDStack.top());
        node.emplace(keyId.value(), value);
    }
}


void ObjectCreator::pushContext(NodePtr node, const std::string& keyStr, State state)
{
    nodeStack.push(node);
    maxMapId += (1 << 16);
    mapIDStack.push(maxMapId);
    keyStack.push(keyStr);
    stateStack.push(state);
}


void ObjectCreator::pushContext(NodePtr node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
}


void ObjectCreator::popContext()
{
    nodeStack.pop();
    keyStack.pop();
    mapIDStack.pop();
    stateStack.pop();
}

