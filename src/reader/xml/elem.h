
#pragma once

#include <map>
#include <string>
#include <optional>

#include "token.h"


namespace xml
{

enum class ElemType
{
    DECLARATION,
    TAG_OPEN,
    TAG_CLOSE,
    TAG_ARRAY_BEGIN,
    TAG_ARRAY_END,
    TAG_ARRAY_OPEN,
    TAG_ARRAY_CLOSE,
    TAG_NULL,
    CONTENT
};


struct ElemReader
{
    ElemReader(ElemType type, const std::string& name) : 
               type(type), name(name), value(nullptr) {}

    ElemReader(ElemType type, const std::string& name, TokenData& value) : 
               type(type), name(name), value(value) {}

    ElemReader(ElemType type, std::optional<std::string> name, TokenData& value) :
               type(type), name(name), value(value) {}

    ElemType type;
    std::optional<std::string> name;
    TokenData value;
    std::map<std::string, std::string> attr ;
};


struct  Elem
{
    Elem(ElemType type, std::optional<std::string> name) :
               type(type), name(name), value(nullptr) {}

    Elem(ElemType type, std::optional<std::string> name, const TokenData& value) :
               type(type), name(name), value(value) {}

    ElemType type;
    std::optional<std::string> name;
    TokenData value;
    std::map<std::string, std::string> attr;
};
}

