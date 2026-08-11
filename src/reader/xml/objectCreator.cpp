
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
            case TAG_ARRAY_CLOSE:
                popContext();
                break;
            case CONTENT:
                processContent(elem.name, elem.value);
                break;
        }
    }
    return document;
}


void ObjectCreator::processTagOpen(const std::string& currKeyStr)
{

    State state = stateStack.top();

    if (state == State::OBJECT_PARSING) 
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
    else if (state == State::ARRAY_PARSING)
    {
        ArrayNode* arr = std::get<ArrayNode*>(nodeStack.top());
        auto&  ref = arr->emplace_back(ObjectNode());
        auto *currNode__ = &(std::get<ObjectNode>(ref.value));
        pushContext(currNode__, currKeyStr, State::OBJECT_PARSING);
    }
}


void ObjectCreator::processTagArrayOpen(const std::string& currKeyStr)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    auto optPrevKey = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
    if (optPrevKey.has_value()) {
        auto *currNode = std::get_if<ObjectNode>(&obj->at(optPrevKey.value()).value);
        pushContext(currNode, currKeyStr, State::ARRAY_PARSING);
        return;
    }

    optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    uint32_t prevKey = optPrevKey.value();

    obj->emplace(prevKey, ObjectNode());
    auto *currNode = std::get_if<ObjectNode>(&obj->at(prevKey).value);
    auto optKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    uint32_t Key = optPrevKey.value();

    currNode->emplace(Key, ArrayNode());
    ArrayNode* currNode_ = std::get_if<ArrayNode>(&currNode->at(Key).value);
    pushContext(currNode_, State::ARRAY_PARSING);
}


void ObjectCreator::processContent(const std::string& contentName, TokenData& data)
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


void ObjectCreator::insertAttrs(ObjectNode& node, std::map<std::string, std::string>& attrs)
{
    std::optional<unsigned int> keyId;
    for (const auto& [keyStr, value] : attrs) {
        keyId = keyMapper.createKeyIDAttr(keyStr, mapIDStack.top());
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

