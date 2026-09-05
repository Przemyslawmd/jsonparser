
#pragma once

#include <memory>
#include <string>

#include "../../headers/elem.h"
#include "keyMapper.h"


namespace xml
{
class Writer
{
public:
    Writer(const KeyMapper& keyMapper, unsigned int indentStep) : keyMapper(keyMapper),
                                                                  indentStep(indentStep) {}

    std::string createXmlString(std::unique_ptr<std::vector<Elem>> elems);
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

