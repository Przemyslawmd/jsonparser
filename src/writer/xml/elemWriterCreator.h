
#ifndef JX_WRITER_XML_ELEM_WRITER_CREATOR_H
#define JX_WRITER_XML_ELEM_WRITER_CREATOR_H

#include <memory>
#include <string>
#include <variant>

#include "reader/xml/elem.h"
#include "node.h"
#include "keyMapper.h"


namespace xml
{
class ElemWriterCreator
{
public:
    ElemWriterCreator() = default;

    std::vector<ElemWriter> createElems(const ObjectNode&);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node&);

    const KeyMapper& keyMapper;
    
    std::vector<ElemWriter> elems;
};
}

#endif

