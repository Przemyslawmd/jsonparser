
#ifndef JX_READER_XML_ELEM_H
#define JX_READER_XML_ELEM_H

#include <map>
#include <string>
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
               type(type), name(name), value(nullptr), attr(std::move(attr)) {}

    ElemReader(ElemType type, const std::string& name) : 
               type(type), name(name), value(nullptr), attr({}) {}

    ElemReader(ElemType type, const std::string& name, VariantData& value) : 
               type(type), name(name), value(value), attr({}) {}

    ElemReader(ElemType type, VariantData& value) : 
               type(type), name({}), value(value), attr({}) {}

    ElemType type;
    std::string name;
    std::vector<xml::Token> attr;
    VariantData value;
    //std::variant<std::string, int64_t, double, bool, nullptr_t> value;
};


struct ElemWriter
{
    ElemType type;
    std::string name;
    std::map<std::string, std::string> attr;
};
}

// std::string, int64_t, double, bool, nullptr_t

#endif

