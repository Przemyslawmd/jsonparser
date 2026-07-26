
#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "token.h"


namespace xml
{
class Preparser
{
public:
    Preparser() = default;

    std::unique_ptr<std::vector<Token>> parseXML(const std::string& json);

private:
    size_t parseStringOutQuotation(const std::string& json, uint index);

    std::unique_ptr<std::vector<Token>> tokens;

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

