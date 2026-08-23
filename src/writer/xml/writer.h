
#pragma once

#include <memory>
#include <string>

#include "reader/xml/elem.h"
#include "keyMapper.h"


namespace xml
{
class Writer
{
public:
    Writer(const KeyMapper& keyMapper, unsigned int indentStep) : keyMapper(keyMapper),
                                                                  indent(0),
                                                                  indentStep(indentStep) {}

    std::string createXmlString(std::unique_ptr<std::vector<ElemWriter>> elems);
    void setIndent(unsigned int);

private:
    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream, unsigned int noOfChars);

    const KeyMapper& keyMapper;
    unsigned int indent;
    unsigned int indentStep;
};
}

