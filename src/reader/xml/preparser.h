#ifndef JX_READER_XML_PREPARSER_H
#define JX_READER_XML_PREPARSER_H

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "token.h"


namespace xml
{
class Preparser
{
public:
    Preparser() = default;

    std::unique_ptr<std::vector<xml::Token>> parseXML(const std::string& json);

private:
    size_t parseStringOutQuotation(const std::string& json, size_t index);

    std::unique_ptr<std::vector<xml::Token>> tokens;

    const std::map<char, TokenType> tokensMap 
    {
        { '=', TokenType::EQUAL },
        { '?', TokenType::QUESTION },
        { '/', TokenType::SLASH },
        { '<', TokenType::ANGLE_OPEN },
        { '>', TokenType::ANGLE_CLOSE },
    };
};
}

#endif

