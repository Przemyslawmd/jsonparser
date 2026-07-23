
#include "objectCreator.h"

#include <ranges>


using namespace xml;

std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<ElemReader>& elems)
{
    uint firstTag = 0;
    if (elems.front().type == ElemType::DECLARATION) {
        keyMapper.storeAttrsDec(std::move(elems.front().attrs));
        firstTag = 1;
    }

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name);
    attrs = &elems.at(firstTag).attrs;

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name);
                attrs = &elem.attrs;
                break;
            case TAG_CLOSE:
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


void ObjectCreator::processContent(const std::string& contentName, VariantData& data)
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
    auto newKeyID = keyMapper.createKeyID("__text", mapIDStack.top());
    std::visit([currentNode, newKeyID](auto&& val) { currentNode->emplace(newKeyID.value(), val); }, data);
}


void ObjectCreator::insertAttrs(ObjectNode& node, std::map<std::string, std::string>& attrs)
{
    std::optional<uint> keyId;
    for (const auto& [key, value] : attrs) {
        keyId = keyMapper.createKeyIDAttr(key, mapIDStack.top());
        node.emplace(keyId.value(), value);
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

