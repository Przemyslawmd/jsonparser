
#ifndef JX_READER_XML_ELEM_H
#define JX_READER_XML_ELEM_H

#include <map>
#include <string>
#include <vector>

#include "token.h"


enum class ElemType
{
    DECLARATION,
    TAG_OPEN,
    TAG_CLOSE,
    CONTENT
};


struct ElemReader
{
    ElemType type;
    std::string name;
    std::vector<xml::Token> attr;
};


struct ElemWriter
{
    ElemType type;
    std::string name;
    std::map<std::string, std::string> attr;
};

#endif

