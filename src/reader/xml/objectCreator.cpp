
#include "objectCreator.h"

#include <ranges>


std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<Elem>& elems)
{
    uint firstTag = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;

    nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushDataOnStack(nodes.get());

    using enum ElemType;
    for (const auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name.value());
                break;
            case TAG_CLOSE:
                popDataFromStack();
                break;
            case CONTENT:
                processContent("X");
                break;
        }
    }
    return std::move(nodes);
}

void ObjectCreator::processTagOpen(const std::string& keyStr)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
    if (optKeyID == std::nullopt) {
        return;
    }
    uint32_t keyID = optKeyID.value();

    obj->emplace(keyID, ObjectNode());
    auto *currentNode = &(std::get<ObjectNode>(obj->at(keyID).value));
    pushDataOnStack(currentNode);
}


void ObjectCreator::processContent(const std::string& keyStr)
{
    auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
    if (optKeyID == std::nullopt) {
        return;
    }
    uint32_t keyID = optKeyID.value();

    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    obj->emplace(keyID, "QAZ");
}


void ObjectCreator::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node)
{
    nodeStack.push(node);
    maxMapId += (1 << 16);
    mapIDStack.push(maxMapId);
}


void ObjectCreator::popDataFromStack()
{
    mapIDStack.pop();
    nodeStack.pop();
}

