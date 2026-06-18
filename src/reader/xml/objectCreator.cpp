
#include "objectCreator.h"

#include <ranges>


std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<Elem>& elems)
{
    uint firstTag = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name.value());

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name.value());
                attrs = &elem.attr;
                break;
            case TAG_CLOSE:
                popContext();
                break;
            case CONTENT:
                processContent(elem.attr);
                break;
        }
    }
    return document;
}


void ObjectCreator::processTagOpen(const std::string& keyStr)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    auto optKeyID = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
    if (optKeyID.has_value()) {
        auto *currentNode = std::get_if<ObjectNode>(&obj->at(optKeyID.value()).value);
        pushContext(currentNode, keyStr);
        return;
    }

    optKeyID = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    if (!optKeyID.has_value()) {
        return;
    }

    uint32_t keyID = optKeyID.value();
    obj->emplace(keyID, ObjectNode());
    auto *currentNode = std::get_if<ObjectNode>(&obj->at(keyID).value);
    pushContext(currentNode, keyStr);
}


void ObjectCreator::processContent(std::vector<TokenXML>& currentAttrs)
{
    auto optKeyID = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    if (!optKeyID.has_value()) {
        return;
    }
    uint32_t keyID = optKeyID.value();
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    if (attrs->empty()) {
        obj->emplace(keyID, std::get<std::string>(currentAttrs[0].data));
        return;
    }

    obj->emplace(keyID, ObjectNode());
    auto *currentNode = std::get_if<ObjectNode>(&obj->at(keyID).value);
    insertAttrs(*currentNode, *attrs);
    auto newKeyID = keyMapper.createKeyID("__text", mapIDStack.top());
    currentNode->emplace(newKeyID.value(), std::get<std::string>(currentAttrs[0].data));
}


void ObjectCreator::insertAttrs(ObjectNode& node, std::vector<TokenXML>& attrs)
{
    constexpr uint FIRST_ATTR_VALUE_INDEX = 2;
    std::optional<uint> keyId;

    for (uint i = FIRST_ATTR_VALUE_INDEX; i < attrs.size(); i++) {
        if (i % 2 == 0) {
            keyId = keyMapper.createKeyID(std::get<std::string>(attrs[i - 2].data), mapIDStack.top());
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

