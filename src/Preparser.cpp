#include <iostream>
#include "Preparser.h"


Preparser::Preparser()
{
    tokens = std::make_unique<std::vector<Token>>();
    tokensMap = {
        {'{', TokenType::CURLY_OPEN},
        {'}', TokenType::CURLY_CLOSE},
        {'[', TokenType::SQUARE_OPEN},
        {']', TokenType::SQUARE_CLOSE},
        {':', TokenType::COLON},
        {',', TokenType::COMMA},
    };
}


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string &json)
{
    for (int i = 0; i < json.length(); i++)
    {
        std::cout << json[i] << std::endl;
    }
    return nullptr;
}


void Preparser::parseNumber()
{
}


void Preparser::parseString()
{
}
