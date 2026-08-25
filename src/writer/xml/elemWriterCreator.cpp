
#include "elemWriterCreator.h"

#include <optional>
#include <ranges>
#include <variant>


using namespace xml;
using enum ElemType;

std::unique_ptr<std::vector<ElemWriter>> ElemWriterCreator::createElems(const ObjectNode& object)
{
    bool const addRoot = xmlRootIfNeeded && object.size() > 1;
    removeElem = false;
    elems = std::make_unique<std::vector<ElemWriter>>();

    if (addRoot) {
        elems->emplace_back(TAG_OPEN, "root");
    }
    processObjectNode(object);
    if (addRoot) {
        elems->emplace_back(TAG_CLOSE, "root");
    }
    return std::move(elems);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

void ElemWriterCreator::processObjectNode(const ObjectNode& obj)
{
    state.push(State::OBJECT_PARSING);
    for (const auto& [idKey, val] : obj) {
        auto keyStr = keyMapper.getKeyStr(idKey);
        if (keyMapper.isAttrKey(idKey)) {
            elems->back().attr.emplace(keyStr.value(), std::get<std::string>(val.value));
            continue;
        }
        if (keyStr == pretendedKey) {
            parseData(val.value);
            continue;
        }

        elems->emplace_back(TAG_OPEN, keyStr.value());
        parseData(val.value);
        if (!removeElem) {
            elems->emplace_back(TAG_CLOSE, keyStr.value());
        }
    }
    removeElem = false;
    state.pop();
}


void ElemWriterCreator::processArrayNode(const ArrayNode& arr)
{
    if (state.top() == State::OBJECT_PARSING) {
        arrayKey = elems->back().name.value();
        elems->pop_back();
    }
    state.push(State::ARRAY_PARSING);

    for (const auto& [idx, a] : std::views::enumerate(arr)) {
        auto elemOpenType  = idx == 0 ? TAG_ARRAY_BEGIN : TAG_ARRAY_OPEN;
        elems->emplace_back(elemOpenType, arrayKey);
        parseData(a.value);
        auto elemCloseType  = idx == (arr.size() - 1) ? TAG_ARRAY_END : TAG_ARRAY_CLOSE;
        elems->emplace_back(elemCloseType, arrayKey);
    }
    removeElem = true;
    state.pop();
}


void ElemWriterCreator::parseData(const Node::Value& nodeVal)
{
    if (std::holds_alternative<std::string>(nodeVal)) {
        elems->emplace_back(CONTENT, std::nullopt, std::get<std::string>(nodeVal));
    }
    else if (std::holds_alternative<int64_t>(nodeVal)) {
        elems->emplace_back(CONTENT, std::nullopt, std::get<int64_t>(nodeVal));
    }
    else if (std::holds_alternative<double>(nodeVal)) {
        elems->emplace_back(CONTENT, std::nullopt, std::get<double>(nodeVal));
    }
    else if (std::holds_alternative<ObjectNode>(nodeVal)) {
        processObjectNode(std::get<ObjectNode>(nodeVal));
    }
    else if (std::holds_alternative<ArrayNode>(nodeVal)) {
        processArrayNode(std::get<ArrayNode>(nodeVal));
    }
}

