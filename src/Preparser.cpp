
#include "Preparser.h"

#include <iostream>
#include <stack>


Preparser::Preparser()
{
    tokens = std::make_unique<std::vector<Token>>();
}


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    error = ParseError::NOT_ERROR;

    if (checkQuotation(json) == false) {
        error = ParseError::STRING_NOT_ENDED;
        return nullptr;
    }

    for (size_t index = 0; index < json.length(); index++)
    {
        char symbol = json[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            index += parseString(json, index);
            if (error != ParseError::NOT_ERROR) {
                return nullptr;
            }
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            index += parseNumber(json, index);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->emplace_back( Token{ tokensMap.at(symbol), nullptr });
            continue;
        }
        if (symbol == 'f') {
            if (json.length() - index > 5 && (json.compare(index, 5, "false") == 0)) {
                tokens->emplace_back(Token{ TokenType::DATA_BOOL, false });
                index += 4;
                continue;
            }
            error = ParseError::UNKNOWN_SYMBOL;
            return nullptr;
        }
        if (symbol == 't') {
            if (json.length() - index > 4 && (json.compare(index, 4, "true") == 0)) {
                tokens->emplace_back(Token{ TokenType::DATA_BOOL, true });
                index += 3;
                continue;
            }
            error = ParseError::UNKNOWN_SYMBOL;
            return nullptr;
        }
        error = ParseError::UNKNOWN_SYMBOL;
        return nullptr;
    }
    return std::move(tokens);
}


size_t Preparser::parseNumber(const std::string& json, size_t index)
{
    int number = 0;
    size_t shift = 1;
    bool isMinus = false;

    if (isdigit(json[index])) {
        number = json[index] - '0';
    }
    else {
        isMinus = true;
    }

    while (index + shift < json.length() && isdigit(json[shift + index])) {
        number = number * 10 + json[shift + index] - '0';
        shift += 1;
    }
    if (json[index + shift] != '.') {
        tokens->emplace_back(Token{ TokenType::DATA_INT, isMinus ? number * -1 : number });
        return shift - 1;
    }

    shift++;
    size_t divider = 1;
    while (index + shift < json.length() && isdigit(json[shift + index])) {
        number = number * 10 + (json[shift + index] - '0');
        shift += 1;
        divider *= 10;
    }
    double numberFloat = (double) number / divider;
    tokens->emplace_back(Token{ TokenType::DATA_DOUBLE, isMinus ? numberFloat * -1.0 : numberFloat });
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

    if (jsonStr[0] == '\"') {
        return false;
    }

    for (size_t index = 1; index < jsonStr.length(); index++) {
        if (jsonStr[index] == '\"' && index > 0 && jsonStr[index - 1] != '\\') {
            quotationCount++;
        }
    }
    return quotationCount % 2 == 0;
}


ParseError Preparser::getError()
{
    return error;
}

