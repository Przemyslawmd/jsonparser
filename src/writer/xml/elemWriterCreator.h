
#pragma once

#include <memory>
#include <string>

#include "reader/xml/elem.h"
#include "node.h"
#include "keyMapper.h"
#include "settings.h"


namespace xml
{
class ElemWriterCreator
{
public:
    ElemWriterCreator(const KeyMapper& keyMapper): keyMapper(keyMapper), 
                                                   pretendedKey(Settings::getPretendedKey()) {}

    std::vector<ElemWriter> createElems(const ObjectNode&);

private:
    void processObjectNode(const ObjectNode&);
    void parseData(const Node&);

    const std::string& pretendedKey;
    const KeyMapper& keyMapper;
    std::vector<ElemWriter> elems;
};
}

