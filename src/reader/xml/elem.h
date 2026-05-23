
#ifndef JSONPARSER_TAG_H
#define JSONPARSER_TAG_H

#include <optional>
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


struct Elem
{
    ElemType type;
    std::optional<std::string> name;
    std::vector<TokenXML> data;
};

#endif

