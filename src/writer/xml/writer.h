
#ifndef JX_WRITER_XML_WRITER_H
#define JX_WRITER_XML_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include "reader/xml/elem.h"
#include "node.h"
#include "keyMapper.h"


namespace xml
{
class Writer
{
public:
    Writer(const KeyMapper& keyMapper) : keyMapper(keyMapper), indent(-2), indentStep(2) {}

    std::string createXmlString(const std::vector<ElemWriter>& elems);
    void setIndent(size_t);

private:
    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream, uint noOfChars);

    const KeyMapper& keyMapper;
    std::ostringstream stream;
    int indent;
    int indentStep;
    std::vector<Elem> elems;
};
}

#endif

