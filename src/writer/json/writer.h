
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
    Writer(const KeyMapper& keyMapper, unsigned int indentStep) : keyMapper(keyMapper),
                                                                  indent(0),
                                                                  indentStep(indentStep) {}

    std::string createJsonString(const ObjectNode&);

    void setIndent(unsigned int);

private:
    void processObjectNode(const ObjectNode&);
    void processArrayNode(const ArrayNode&);
    void parseData(const Node::Value&);

    void incIndent();
    void decIndent();

    const KeyMapper& keyMapper;

    std::ostringstream stream;
    unsigned int indent;
    unsigned int indentStep;
};
}

