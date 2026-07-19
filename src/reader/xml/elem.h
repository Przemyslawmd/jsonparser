
#ifndef JX_READER_XML_ELEM_H
#define JX_READER_XML_ELEM_H

#include <map>
#include <string>
#include <optional>
#include <vector>

#include "token.h"


namespace xml
{

enum class ElemType
{
    DECLARATION,
    TAG_OPEN,
    TAG_CLOSE,
    CONTENT
};


struct ElemReader
{
    ElemReader(ElemType type, const std::string& name, std::vector<xml::Token>&& attr) : 
               type(type), name(name), value(nullptr), attr(std::move(attr)), attrs({})  {}

    ElemReader(ElemType type, const std::string& name) : 
               type(type), name(name), value(nullptr), attr({}), attrs({}) {}

    ElemReader(ElemType type, const std::string& name, VariantData& value) : 
               type(type), name(name), value(value), attr({}), attrs({}) {}

    ElemReader(ElemType type, VariantData& value) : 
               type(type), name({}), value(value), attr({}), attrs({}) {}

    ElemType type;
    std::string name;
    std::vector<xml::Token> attr;
    std::map<std::string, std::string> attrs;
    VariantData value;
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
    VariantData value;
};
}

#endif

