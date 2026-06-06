
#include "objectCreator.h"

#include <ranges>


std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<Elem>& elems)
{
    uint firstTag = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;

    nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    keyStack.push(elems.at(firstTag).name.value());
    pushDataOnStack(nodes.get());
    flagContent = false;

    using enum ElemType;
    for (const auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name.value(), elem.attr);
                break;
            case TAG_CLOSE:
                popDataFromStack();
                break;
            case CONTENT:
                processContent(elem.attr);
                break;
        }
    }
    return std::move(nodes);
}


void ObjectCreator::processTagOpen(const std::string& keyStr, const std::vector<TokenXML>& attrs)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

    auto optKeyID = keyMapper.createAndPutKeyID(keyStack.top(), mapIDStack.top());
    if (optKeyID == std::nullopt) {
        return;
    }
    uint32_t keyID = optKeyID.value();

    if (obj->empty()) {
        obj->emplace(keyID, ObjectNode());
        auto *currentNode = &(std::get<ObjectNode>(obj->at(keyID).value));
        pushDataOnStack(currentNode);
    }
    keyStack.push(keyStr);
}


void ObjectCreator::processContent(const std::vector<TokenXML>& attrs)
{
    auto optKeyID = keyMapper.createAndPutKeyID(keyStack.top(), mapIDStack.top());
    if (optKeyID == std::nullopt) {
        return;
    }
    uint32_t keyID = optKeyID.value();

    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    obj->emplace(keyID, std::get<std::string>(attrs[0].data));
    flagContent = true;
}


void ObjectCreator::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node)
{
    nodeStack.push(node);
    maxMapId += (1 << 16);
    mapIDStack.push(maxMapId);
}


void ObjectCreator::popDataFromStack()
{
    if (flagContent) {
        flagContent = false;
        keyStack.pop();
        return;
    }
    nodeStack.pop();
    keyStack.pop();
    mapIDStack.pop();
}

