
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
    Writer(const KeyMapper& keyMapper, int indentation) : keyMapper(keyMapper),
                                                          indentation(indentation * -1),
                                                          indentationStep(indentation) {}

    std::string createXmlString(std::unique_ptr<std::vector<ElemWriter>> elems);
    void setIndent(int);

private:
    void incIndent();
    void decIndent();

    void deleteLastChars(std::ostringstream& stream, unsigned int noOfChars);

    const KeyMapper& keyMapper;
    int indentation;
    unsigned int indentationStep;
    std::vector<ElemReader> elems;
};
}

