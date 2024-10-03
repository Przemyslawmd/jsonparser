
#ifndef JSONPARSER_WRITER_H
#define JSONPARSER_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include <defines.h>
#include <Error.h>
#include <NodeValue.h>
#include "../keyMapper.h"


class Writer
{
public:

    Writer(const KeyMapper& keyMapper) : keyMapper(keyMapper) {}

    std::optional<std::string> createJsonString(const ObjectNode&);

    void setIndent(size_t);

private:

    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const NodeInternal&);

    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream);

    const KeyMapper& keyMapper;

    std::ostringstream stream;
    size_t indent = 0;
    size_t indentStep = 2;
};

#endif

