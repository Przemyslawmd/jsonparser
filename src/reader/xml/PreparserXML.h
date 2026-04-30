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

    std::unique_ptr<std::vector<TokenXML>> parseXML(const std::string& json);

private:
    size_t parseNumber(const std::string& json, size_t index);
    size_t parseString(const std::string& json, size_t index);

    std::unique_ptr<std::vector<TokenXML>> tokens;

    const std::map<char, TokenTypeXML> tokensMap {
        { '<', TokenTypeXML::TAG_OPEN },
        { '>', TokenTypeXML::TAG_CLOSE },
        { '/', TokenTypeXML::SLASH },
    };
};

#endif