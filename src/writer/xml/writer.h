
#ifndef JX_WRITER_XML_WRITER_H
#define JX_WRITER_XML_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include "node.h"
#include "keyMapper.h"


namespace xml
{
class Writer
{
public:
    Writer(const KeyMapper& keyMapper) : keyMapper(keyMapper) {}

    std::string createXmlString(const ObjectNode&);
    void setIndent(size_t);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node&);

    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream, uint noOfChars);

    const KeyMapper& keyMapper;
    std::ostringstream stream;
    int indent = -2;
    int indentStep = 2;
};
}

#endif

