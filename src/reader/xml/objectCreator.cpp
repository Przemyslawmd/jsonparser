
#include "objectCreator.h"

#include <ranges>


std::unique_ptr<ObjectNode> ObjectCreator::parseElems(std::vector<Elem>& elems)
{
    uint firstTag = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;
    size_t key = 0;

    nodes = std::make_unique<ObjectNode>();
    pushDataOnStack(nodes.get());

    using enum ElemType;
    for (const auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(key);
                break;
            case TAG_CLOSE:
                popDataFromStack();
                break;
            case CONTENT:
                processContent(key);
                break;
        }
    }
    return std::move(nodes);
}

void ObjectCreator::processTagOpen(uint key)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    obj->emplace(key, ObjectNode());
    auto *currentNode = &(std::get<ObjectNode>(obj->at(key).value));
    pushDataOnStack(currentNode);
    key++;
}


void ObjectCreator::processContent(uint key)
{
    ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
    obj->emplace(key, "QAZ");
}


void ObjectCreator::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node)
{
    nodeStack.push(node);
}


void ObjectCreator::popDataFromStack()
{
    nodeStack.pop();
}

