
#include "Preparser.h"

#include <format>
#include <iostream>
#include <stack>


constexpr size_t lettersInFalse = 5;
constexpr size_t lettersInTrue = 4;
constexpr size_t lettersInNull = 4;


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    tokens = std::make_unique<std::vector<Token>>();

    for (size_t index = 0; index < json.length(); index++)
    {
        char symbol = json[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            size_t shift = parseString(json, index);
            if (shift == 0) {
                return nullptr;
            }
            index += shift;
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
            if (json.length() - index > lettersInFalse && (json.compare(index, lettersInFalse, "false") == 0)) {
                tokens->emplace_back(Token{ TokenType::DATA_BOOL, false });
                index += (lettersInFalse - 1);
                continue;
            }
            error = std::make_unique<Error>(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 't') {
            if (json.length() - index > lettersInTrue && (json.compare(index, lettersInTrue, "true") == 0)) {
                tokens->emplace_back(Token{ TokenType::DATA_BOOL, true });
                index += (lettersInTrue -1);
                continue;
            }
            error = std::make_unique<Error>(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 'n') {
            if (json.length() - index > lettersInNull && (json.compare(index, lettersInNull, "null") == 0)) {
                tokens->emplace_back(Token{ TokenType::DATA_NULL, nullptr });
                index += (lettersInNull - 1);
                continue;
            }
            error = std::make_unique<Error>(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        error = std::make_unique<Error>(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
        return nullptr;
    }
    return std::move(tokens);
}


std::unique_ptr<Error> Preparser::getError()
{
    return std::move(error);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

size_t Preparser::parseNumber(const std::string& json, size_t index)
{
    int64_t number = 0;
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
    error = std::make_unique<Error>(ErrorCode::PREPARSER_STRING_ERROR);
    return 0;
}

