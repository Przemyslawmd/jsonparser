
#include "writer/xml/elemWriterCreator.h"

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
        auto keyStr = keyMapper.getKeyStr(idKey);
        if (keyMapper.isAttrKey(idKey)) {
            elems.at(elems.size() - 1).attr.emplace(keyStr.value(), std::get<std::string>(val.value));
            continue;
        }
        if (keyStr == "__text") {
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
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        processObjectNode(std::get<ObjectNode>(node.value));
    }
}

