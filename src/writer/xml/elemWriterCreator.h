
#pragma once

#include <stack>
#include <string>

#include "reader/xml/elem.h"
#include "node.h"
#include "keyMapper.h"
#include "settings.h"
#include "state.h"


namespace xml
{
class ElemWriterCreator
{
public:
    explicit ElemWriterCreator(const KeyMapper& keyMapper): keyMapper(keyMapper),
                                                            pretendedKey(Settings::getPretendedKey()),
                                                            xmlRootIfNeeded(Settings::getXmlRoot()) {}

    std::vector<ElemWriter> createElems(const ObjectNode&);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node::Value&);

    const KeyMapper& keyMapper;

    const std::string& pretendedKey;
    const bool xmlRootIfNeeded;

    std::vector<ElemWriter> elems;
    std::string arrayKey;
    bool removeElem;
    std::stack<State> state;
};
}

