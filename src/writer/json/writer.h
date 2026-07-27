
#pragma once

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
    Writer(const KeyMapper& keyMapper, uint indentationStep) : keyMapper(keyMapper), 
                                                               indentation(0), 
                                                               indentationStep(indentationStep) {}

    std::string createJsonString(const ObjectNode&);

    void setIndent(uint);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node&);

    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream);

    const KeyMapper& keyMapper;

    std::ostringstream stream;
    uint indentation;
    uint indentationStep;
};
}

