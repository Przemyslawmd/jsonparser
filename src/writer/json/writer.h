
#ifndef JX_WRITER_JSON_WRITER_H
#define JX_WRITER_JSON_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include "node.h"
#include "keyMapper.h"


namespace json
{
class Writer
{
public:
    Writer(const KeyMapper& keyMapper) : keyMapper(keyMapper), indent(0), indentStep(2) {}

    std::string createJsonString(const ObjectNode&);

    void setIndent(size_t);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node&);

    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream);

    const KeyMapper& keyMapper;

    std::ostringstream stream;
    size_t indent;
    size_t indentStep;
};
}

#endif

