
#include "parserElems.h"

#include <ranges>


using namespace xml;
using enum State;


std::unique_ptr<ObjectNode> ParserElems::parseElems(std::vector<Elem>& elems)
{
    unsigned int firstTag = 0;
    if (elems.front().type == ElemType::DECLARATION) {
        keyMapper.storeAttrsDec(std::move(elems.front().attrs));
        firstTag = 1;
    }

    auto document = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushContext(document.get(), elems.at(firstTag).name.value(), OBJECT_PARSING);
    attrs = &elems.at(firstTag).attrs;

    using enum ElemType;
    for (auto& elem : elems | std::views::drop(firstTag + 1)) {
        switch (elem.type) {
            case TAG_OPEN:
                processTagOpen(elem.name.value());
                attrs = &elem.attrs;
                break;
            case TAG_ARRAY_BEGIN:
                processTagArrayOpen(elem.name.value());
                attrs = &elem.attrs;
                break;
            case TAG_CLOSE:
                popContext();
                break;
            case TAG_ARRAY_END:
                arrStack.pop();
                stateStack.pop();
                popContext();
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


void ParserElems::processTagOpen(const std::string& key)
{
    if (stateStack.top() == OBJECT_PARSING)
    {
        ObjectNode* objNode = objStack.top();
        auto optPrevKey = keyMapper.getKeyID(keyStack.top(), mapIDStack.top());
        if (optPrevKey.has_value()) {
            ObjectNode* currNode = std::get_if<ObjectNode>(&objNode->at(optPrevKey.value()).value);
            pushContext(currNode, key, OBJECT_PARSING);
            return;
        }

        optPrevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        uint32_t prevKey = optPrevKey.value();
        objNode->emplace(prevKey, ObjectNode());
        ObjectNode* newNode = std::get_if<ObjectNode>(&objNode->at(prevKey).value);

        if (attrs && !attrs->empty()) {
            insertAttrs(*newNode, *attrs);
        }
        pushContext(newNode, key, OBJECT_PARSING);
    }
    else {
        ArrayNode* arrNode = arrStack.top();
        Node& ref = arrNode->emplace_back(ObjectNode());
        ObjectNode* newNode = std::get_if<ObjectNode>(&ref.value);
        pushContext(newNode, key, OBJECT_PARSING);
    }
}


void ParserElems::processTagArrayOpen(const std::string& key)
{
    ObjectNode* objNode = objStack.top();
    auto prevKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
    objNode->emplace(prevKey.value(), ObjectNode());
    ObjectNode* currNode = std::get_if<ObjectNode>(&objNode->at(prevKey.value()).value);
    pushContext(currNode, key, OBJECT_PARSING);

    auto arrKey = keyMapper.createKeyID(key, mapIDStack.top());
    currNode->emplace(arrKey.value(), ArrayNode());
    ArrayNode* arrNode = std::get_if<ArrayNode>(&currNode->at(arrKey.value()).value);
    pushContext(arrNode, ARRAY_PARSING);
}


void ParserElems::processContent(TokenData& data)
{
    if (stateStack.top() == OBJECT_PARSING)
    {
        auto optKey = keyMapper.createKeyID(keyStack.top(), mapIDStack.top());
        if (!optKey.has_value()) {
            return;
        }
        uint32_t keyID = optKey.value();
        ObjectNode* objNode = objStack.top();

        if (attrs && attrs->empty()) {
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
        ArrayNode* arrNode  = arrStack.top();
        std::visit([arrNode](auto&& val) { arrNode->emplace_back(val); }, data);
    }
}


void ParserElems::insertAttrs(ObjectNode& node, std::vector<std::tuple<std::string, std::string>>& attrs)
{
    for (const auto& attr : attrs) {
        auto keyId = keyMapper.createKeyIDAttr(std::get<0>(attr), mapIDStack.top());
        node.emplace(keyId.value(), std::get<1>(attr));
    }
}


template <typename T> requires ComplexNodeObjectCreator<T>
void ParserElems::pushContext(T* node, const std::string& key, State state)
{
    if constexpr (std::same_as<T, ObjectNode>) {
        objStack.push(node);
    }
    else {
        arrStack.push(node);
    }
    maxMapId += (1 << 16);
    mapIDStack.push(maxMapId);
    keyStack.push(key);
    stateStack.push(state);
}


void ParserElems::pushContext(ArrayNode* node, State state)
{
    arrStack.push(node);
    stateStack.push(state);
}


void ParserElems::popContext()
{
    if (stateStack.top() == OBJECT_PARSING) {
        objStack.pop();
    }
    else {
        arrStack.pop();
    }
    keyStack.pop();
    mapIDStack.pop();
    stateStack.pop();
}

