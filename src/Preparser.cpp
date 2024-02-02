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


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    int shift = 0;
    for (int i = 0; i < json.length(); i++)
    {
        if (json[i] == '\'' || json[i] == '\"') {
            shift = parseString(json, i);
            i += shift;
        }
        if (isdigit(json[i])) {
            shift = parseNumber(json, i);
            i += shift;
        }
        if (tokensMap.count(json[i])) {
            Token token;
            token.type = tokensMap.at(json[i]);
            token.data = nullptr;
            tokens->push_back(token);
        }
    }
    return std::move(tokens);
}


int Preparser::parseNumber(const std::string& json, int index)
{
    int number = json[index] - '0';
    int shift = 1;
    while (index + shift < json.length() && isdigit(json[shift + index])) {
        number = number * 10 + json[shift + index] - '0';
        shift += 1;
    }
    Token token;
    token.type = TokenType::DATA_INT;
    token.data = number;
    tokens->push_back(token);
    return shift;    
}


int Preparser::parseString(const std::string& json, int index)
{
    int shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
                Token token;
                token.type = TokenType::DATA_STR;
                token.data = json.substr(index + 1, shift - 1);
                tokens->push_back(token);
                return shift + 1;
        }
        shift += 1;
    }
    return -1;
}
