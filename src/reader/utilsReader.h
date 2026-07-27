
#pragma once

#include <string>

#include "errorCode.h"
#include "log/ErrorStorage.h"
#include "token.h"


template <typename T, typename U>
concept ConceptToken = std::same_as<T, json::Token> && std::same_as<U, json::TokenType> ||
                       std::same_as<T, xml::Token> && std::same_as<U, xml::TokenType>;


static uint parseString(const std::string& file, uint index)
{
    uint shift = 1;
    while (index + shift < file.length()) {
        if (file[index + shift] == '\"') {
            return shift;
        }
        shift += 1;
    }
    ErrorStorage::putError(ErrorCode::PREPARSER_STRING_ERROR);
    return 0;
}


template <typename T, typename U> requires ConceptToken<T, U>
size_t parseNumber(const std::string& json, size_t index, std::vector<T>& tokens)
{
    int64_t number = 0;
    bool isMinus = false;

    if (isdigit(json[index])) {
        number = json[index] - '0';
    }
    else {
        isMinus = true;
    }

    index++;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + json[index] - '0';
        index++;
    }
    if (json[index] != '.') {
        tokens.emplace_back(U::DATA_INT, isMinus ? number * -1 : number);
        return index - 1;
    }

    index++;
    size_t divider = 1;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + (json[index] - '0');
        index++;
        divider *= 10;
    }
    double numberFloat = (double) number / divider;
    tokens.emplace_back(U::DATA_DOUBLE, isMinus ? numberFloat * -1.0 : numberFloat);
    return index - 1;
}

