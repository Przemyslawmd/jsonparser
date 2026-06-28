#ifndef JSONPARSER_PREPARSER_XML_H
#define JSONPARSER_PREPARSER_XML_H

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "token.h"


class PreparserXML
{
public:
    PreparserXML() = default;

    std::unique_ptr<std::vector<xml::Token>> parseXML(const std::string& json);

private:
    size_t parseStringOutQuotation(const std::string& json, size_t index);

    std::unique_ptr<std::vector<xml::Token>> tokens;

    const std::map<char, TokenTypeXML> tokensMap 
    {
        { '=', TokenTypeXML::EQUAL },
        { '?', TokenTypeXML::QUESTION },
        { '/', TokenTypeXML::SLASH },
        { '<', TokenTypeXML::ANGLE_OPEN },
        { '>', TokenTypeXML::ANGLE_CLOSE },
    };
};

#endif

