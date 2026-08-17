
#include "objectCreator.h"

#include <ranges>


using namespace xml;

std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<ElemReader>& elems)
{
    unsigned int firstTag = 0;
    if (elems.front().type == ElemType::DECLARATION) {
        keyMapper.storeAttrsDec(std::move(elems.front().attrs));
        firstTag = 1;
    }

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name, State::OBJECT_PARSING);
    attrs = &elems.at(firstTag).attrs;

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name);
                attrs = &elem.attrs;
                break;
            case TAG_ARRAY_OPEN:
                processTagArrayOpen(elem.name);
                attrs = &elem.attrs;
                break;
            case TAG_CLOSE:
                popContext();
                break;
            case TAG_ARRAY_CLOSE:
                nodeStack.pop();
                stateStack.pop();
                keyStack.pop();
                break;
            case CONTENT:
                processContent(elem.value);
                break;
            case TAG_NULL:
                break;
        }
    }
    return document;
}


void ObjectCreator::processTagOpen(const std::string& currKeyStr)
{
    if (stateStack.top() == State::OBJECT_PARSING)
    {
        ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
        auto optPrevKey = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
        if (optPrevKey.has_value()) {
            auto *currNode = std::get_if<ObjectNode>(&obj->at(optPrevKey.value()).value);
            pushContext(currNode, currKeyStr, State::OBJECT_PARSING);
            return;
        }

        optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        uint32_t prevKey = optPrevKey.value();
        obj->emplace(prevKey, ObjectNode());
        auto *currNode = std::get_if<ObjectNode>(&obj->at(prevKey).value);

        if (attrs && !attrs->empty()) {
            insertAttrs(*currNode, *attrs);
        }
        pushContext(currNode, currKeyStr, State::OBJECT_PARSING);
    }
    else {
        auto* arr = std::get<ArrayNode*>(nodeStack.top());
        auto& ref = arr->emplace_back(ObjectNode());
        auto* currNode = std::get_if<ObjectNode>(&ref.value);
        pushContext(currNode, currKeyStr, State::OBJECT_PARSING);
    }
}


void ObjectCreator::processTagArrayOpen(const std::string& currKeyStr)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    auto optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    uint32_t prevKey = optPrevKey.value();
    obj->emplace(prevKey, ObjectNode());
    auto *currNode = std::get_if<ObjectNode>(&obj->at(prevKey).value);
    pushContext(currNode, currKeyStr, State::OBJECT_PARSING);

    auto optKey = keyMapper.createKeyID(currKeyStr, mapIDStack.top());
    uint32_t Key = optKey.value();

    currNode->emplace(Key, ArrayNode());
    ArrayNode* arrNode_ = std::get_if<ArrayNode>(&currNode->at(Key).value);
    pushContext(arrNode_, State::ARRAY_PARSING);
}


void ObjectCreator::processContent(TokenData& data)
{
    if (stateStack.top() == State::OBJECT_PARSING) 
    {
        auto optKeyID = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        if (!optKeyID.has_value()) {
            return;
        }
        uint32_t keyID = optKeyID.value();
        ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

        if (attrs->empty()) {
            std::visit([obj, keyID](auto&& val) { obj->emplace(keyID, val); }, data); 
            return;
        }

        obj->emplace(keyID, ObjectNode());
        auto *currentNode = std::get_if<ObjectNode>(&obj->at(keyID).value);
        insertAttrs(*currentNode, *attrs);
        auto newKeyID = keyMapper.createKeyID(pretendedKey, mapIDStack.top());
        std::visit([currentNode, newKeyID](auto&& val) { currentNode->emplace(newKeyID.value(), val); }, data);
    } 
    else {
        ArrayNode* arr  = std::get<ArrayNode*>(nodeStack.top());
        std::visit([arr](auto&& val) { arr->emplace_back(val); }, data);
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

