#include <iostream>
#include "Preparser.h"


Preparser::Preparser()
{
    tokens = std::make_unique<std::vector<Token>>();
    tokensMap = {
        { '{', TokenType::CURLY_OPEN },
        { '}', TokenType::CURLY_CLOSE },
        { '[', TokenType::SQUARE_OPEN },
        { ']', TokenType::SQUARE_CLOSE },
        { ':', TokenType::COLON },
        { ',', TokenType::COMMA },
    };
}


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    for (size_t i = 0; i < json.length(); i++)
    {
        char symbol = json[i];
        if (symbol == ' ') {
            continue;
        }
        if (symbol == '\'' || symbol == '\"') {
            i += parseString(json, i);
            continue;
        }
        if (isdigit(symbol)) {
            i += parseNumber(json, i);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->push_back(Token{ tokensMap.at(symbol), nullptr });
        }
    }
    return std::move(tokens);
}


int Preparser::parseNumber(const std::string& json, int index)
{
    int number = json[index] - '0';
    size_t shift = 1;
    while (index + shift < json.length() && isdigit(json[shift + index])) {
        number = number * 10 + json[shift + index] - '0';
        shift += 1;
    }
    tokens->push_back(Token{ TokenType::DATA_INT, number });
    return shift - 1;    
}


int Preparser::parseString(const std::string& json, int index)
{
    size_t shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
            tokens->emplace_back(Token{ TokenType::DATA_STR, json.substr(index + 1, shift - 1) });
            return shift;
        }
        shift += 1;
    }
    return -1;
}
