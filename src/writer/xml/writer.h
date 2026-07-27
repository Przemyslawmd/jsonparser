
#pragma once

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
    Writer(const KeyMapper& keyMapper, unsigned int indentation) : keyMapper(keyMapper), 
                                                                   indentation(indentation * -1), 
                                                                   indentationStep(indentation) {}

    std::string createXmlString(const std::vector<ElemWriter>& elems);
    void setIndent(int);

private:
    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream, uint noOfChars);

    const KeyMapper& keyMapper;
    int indentation;
    unsigned int indentationStep;
    std::vector<ElemReader> elems;
};
}

