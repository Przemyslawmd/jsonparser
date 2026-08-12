
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
    TAG_ARRAY_OPEN,
    TAG_ARRAY_CLOSE,
    TAG_NULL,
    CONTENT
};


struct ElemReader
{
    ElemReader(ElemType type, const std::string& name) : 
               type(type), name(name), value(nullptr), attrs({}) {}

    ElemReader(ElemType type, const std::string& name, TokenData& value) : 
               type(type), name(name), value(value), attrs({}) {}

    ElemReader(ElemType type, TokenData& value) : 
               type(type), name({}), value(value), attrs({}) {}

    ElemType type;
    std::string name;
    std::map<std::string, std::string> attrs;
    TokenData value;
};


struct  ElemWriter
{
    ElemWriter(ElemType type, std::optional<std::string> name) : 
               type(type), name(name), value(nullptr), attr({}) {}

    ElemWriter(ElemType type, std::optional<std::string> name, const std::string& value) : 
               type(type), name(name), value(value), attr({}) {}

    ElemWriter(ElemType type, int64_t value) : 
               type(type), name(std::nullopt), value(value), attr({}) {}

    ElemWriter(ElemType type, double value) : 
               type(type), name(std::nullopt), value(value), attr({}) {}

    ElemType type;
    std::optional<std::string> name;
    std::map<std::string, std::string> attr;
    TokenData value;
};
}

