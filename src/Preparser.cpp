
#include "Preparser.h"

#include <iostream>


Preparser::Preparser()
{
    tokens = std::make_unique<std::vector<PreToken>>();
    tokensMap = {
        { '{', TokenType::CURLY_OPEN },
        { '}', TokenType::CURLY_CLOSE },
        { '[', TokenType::SQUARE_OPEN },
        { ']', TokenType::SQUARE_CLOSE },
        { ':', TokenType::COLON },
        { ',', TokenType::COMMA },
    };
}


std::unique_ptr<std::vector<PreToken>> Preparser::parseJSON(const std::string& json)
{
    error = ParseError::NOT_ERROR;

    for (size_t index = 0; index < json.length(); index++)
    {
        char symbol = json[index];
        if (symbol == ' ') {
            continue;
        }
        if (symbol == '\'' || symbol == '\"') {
            index += parseString(json, index);
            if (error != ParseError::NOT_ERROR) {
                return nullptr;
            }
            continue;
        }
        if (isdigit(symbol)) {
            index += parseNumber(json, index);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->push_back(PreToken{ tokensMap.at(symbol), nullptr });
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
    tokens->push_back(PreToken{ TokenType::DATA_INT, number });
    return shift - 1;
}


int Preparser::parseString(const std::string& json, int index)
{
    size_t shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
            tokens->emplace_back(PreToken{ TokenType::DATA_STR, json.substr(index + 1, shift - 1) });
            return shift;
        }
        shift += 1;
    }
    error = ParseError::STRING_NOT_ENDED;
    return 0;
}


ParseError Preparser::getError()
{
    return error;
}

