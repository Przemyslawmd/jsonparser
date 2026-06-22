
#include "writer/xml/elemWriterCreator.h"

#include <iterator>
#include <optional>
#include <variant>


using namespace xml;

std::vector<ElemWriter> ElemWriterCreator::createElems(const ObjectNode& object)
{
    processObjectNode(object);
    return elems;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

void ElemWriterCreator::processObjectNode(const ObjectNode& obj)
{
    for (auto const& [idKey, val] : obj) {
        if (keyMapper.isAttrKey(idKey)) {
            continue;
        }

        auto keyStr = keyMapper.getKeyStr(idKey);
        if (keyStr.value() == "__text") {
            parseData(val);
            continue;
        }

        elems.push_back({ ElemType::TAG_OPEN, keyStr, {}});
        parseData(val);
        elems.push_back({ ElemType::TAG_CLOSE, keyStr, {}});
    }
}


void ElemWriterCreator::parseData(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        elems.push_back({ ElemType::CONTENT, std::get<std::string>(node.value), {}});
    }
    else if (std::holds_alternative<bool>(node.value)) {
    }
    else if (std::holds_alternative<nullptr_t>(node.value)) {
    }
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        processObjectNode(std::get<ObjectNode>(node.value));
    }
}

