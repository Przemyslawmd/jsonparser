
#include "objectCreator.h"

#include <ranges>


std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<ElemReader>& elems)
{
    uint firstTag = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name);
    attrs = &elems.at(firstTag).attr;

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name);
                attrs = &elem.attr;
                break;
            case TAG_CLOSE:
                popContext();
                break;
            case CONTENT:
                processContent(elem.name);
                break;
        }
    }
    return document;
}


void ObjectCreator::processTagOpen(const std::string& currKeyStr)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    auto optPrevKey = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
    if (optPrevKey.has_value()) {
        auto *currNode = std::get_if<ObjectNode>(&obj->at(optPrevKey.value()).value);
        pushContext(currNode, currKeyStr);
        return;
    }

    optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    uint32_t prevKey = optPrevKey.value();

    obj->emplace(prevKey, ObjectNode());
    auto *currNode = std::get_if<ObjectNode>(&obj->at(prevKey).value);

    if (attrs && !attrs->empty()) {
        insertAttrs(*currNode, *attrs);
    }
    pushContext(currNode, currKeyStr);
}


void ObjectCreator::processContent(const std::string& contentName)
{
    auto optKeyID = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    if (!optKeyID.has_value()) {
        return;
    }
    uint32_t keyID = optKeyID.value();
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    if (attrs->empty()) {
        obj->emplace(keyID, contentName);
        return;
    }

    obj->emplace(keyID, ObjectNode());
    auto *currentNode = std::get_if<ObjectNode>(&obj->at(keyID).value);
    insertAttrs(*currentNode, *attrs);
    auto newKeyID = keyMapper.createKeyID("__text", mapIDStack.top());
    currentNode->emplace(newKeyID.value(), contentName);
}


void ObjectCreator::insertAttrs(ObjectNode& node, std::vector<xml::Token>& attrs)
{
    constexpr uint FIRST_ATTR_VALUE_INDEX = 2;
    std::optional<uint> keyId;

    for (uint i = FIRST_ATTR_VALUE_INDEX; i < attrs.size(); i++) {
        if (i % 2 == 0) {
            keyId = keyMapper.createKeyIDAttr(std::get<std::string>(attrs[i - 2].data), mapIDStack.top());
            node.emplace(keyId.value(), std::get<std::string>(attrs[i].data));
        }
    }
}


void ObjectCreator::pushContext(NodePtr node, const std::string& keyStr)
{
    nodeStack.push(node);
    maxMapId += (1 << 16);
    mapIDStack.push(maxMapId);
    keyStack.push(keyStr);
}


void ObjectCreator::popContext()
{
    nodeStack.pop();
    keyStack.pop();
    mapIDStack.pop();
}

