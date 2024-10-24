
#include "Preparser.h"

#include <format>

#include "error.h"
#include "../log/ErrorStorage.h"


constexpr size_t FalseLength = 5;
constexpr size_t TrueLength = 4;
constexpr size_t NullLength = 4;


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    tokens = std::make_unique<std::vector<Token>>();
    tokens->reserve(100);

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
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        if (symbol == 'f') {
            if (json.length() - index > FalseLength && (json.compare(index, FalseLength, "false") == 0)) {
                tokens->emplace_back(TokenType::DATA_BOOL, false);
                index += (FalseLength - 1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 't') {
            if (json.length() - index > TrueLength && (json.compare(index, TrueLength, "true") == 0)) {
                tokens->emplace_back(TokenType::DATA_BOOL, true);
                index += (TrueLength -1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 'n') {
            if (json.length() - index > NullLength && (json.compare(index, NullLength, "null") == 0)) {
                tokens->emplace_back(TokenType::DATA_NULL, nullptr);
                index += (NullLength - 1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
        return nullptr;
    }
    tokens->shrink_to_fit();
    return std::move(tokens);
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
        tokens->emplace_back(TokenType::DATA_INT, isMinus ? number * -1 : number);
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
    tokens->emplace_back(TokenType::DATA_DOUBLE, isMinus ? numberFloat * -1.0 : numberFloat);
    return shift - 1;
}


size_t Preparser::parseString(const std::string& json, size_t index)
{
    size_t shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
            tokens->emplace_back(TokenType::DATA_STR, json.substr(index + 1, shift - 1));
            return shift;
        }
        shift += 1;
    }
    ErrorStorage::putError(ErrorCode::PREPARSER_STRING_ERROR);
    return 0;
}

