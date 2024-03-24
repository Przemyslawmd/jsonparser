
#include "Preparser.h"

#include <iostream>


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


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& jsonStr)
{
    error = ParseError::NOT_ERROR;

    if (checkQuotation(jsonStr) == false) {
        error = ParseError::STRING_NOT_ENDED;
        return nullptr;
    }

    for (size_t index = 0; index < jsonStr.length(); index++)
    {
        char symbol = jsonStr[index];
        if (symbol == ' ') {
            continue;
        }
        if (symbol == '\"') {
            index += parseString(jsonStr, index);
            if (error != ParseError::NOT_ERROR) {
                return nullptr;
            }
            continue;
        }
        if (isdigit(symbol)) {
            index += parseNumber(jsonStr, index);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->push_back(Token{ tokensMap.at(symbol), nullptr });
            continue;
        }
        if (symbol == 'f') {
            if (jsonStr.length() - index > 5 && (jsonStr.compare(index, 5, "false") == 0)) {
                tokens->push_back(Token{ TokenType::DATA_BOOL, false });
                continue;
            }
            error = ParseError::UNKNOWN_SYMBOL;
            return nullptr;
        }
        if (symbol == 't') {
            if (jsonStr.length() - index > 4 && (jsonStr.compare(index, 4, "true") == 0)) {
                tokens->push_back(Token{ TokenType::DATA_BOOL, true });
                continue;
            }
            error = ParseError::UNKNOWN_SYMBOL;
            return nullptr;
        }

    }
    return std::move(tokens);
}


size_t Preparser::parseNumber(const std::string& json, size_t index)
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


size_t Preparser::parseString(const std::string& json, size_t index)
{
    size_t shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
            tokens->emplace_back(Token{ TokenType::DATA_STR, json.substr(index + 1, shift - 1) });
            return shift;
        }
        shift += 1;
    }
    error = ParseError::STRING_NOT_ENDED;
    return 0;
}


bool Preparser::checkQuotation(const std::string& jsonStr)
{
    size_t quotationCount = 0;

    for (size_t index = 0; index < jsonStr.length(); index++) {
        if (jsonStr[index] == '\"' && index > 0 && jsonStr[index - 1] != '\\') {
            quotationCount++;
        }
    }
    std::cout << quotationCount << std::endl;
    return quotationCount % 2 == 0;
}


ParseError Preparser::getError()
{
    return error;
}

