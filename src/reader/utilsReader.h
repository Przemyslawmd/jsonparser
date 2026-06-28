
#ifndef JX_READER_UTILS_READER_H
#define JX_READER_UTILS_READER_H

#include <string>

#include "error.h"
#include "log/ErrorStorage.h"
#include "token.h"


template <typename T, typename U>
concept ConceptToken = std::is_same<T, json::Token>::value || std::is_same<T, xml::Token>::value &&
                       std::is_same<U, json::TokenType>::value || std::is_same<U, xml::TokenType>::value;


template <typename T, typename U> requires ConceptToken<T, U>
static size_t parseString(const std::string& file, size_t index, std::vector<T>& tokens, U type)
{
    size_t shift = 1;
    while (index + shift < file.length()) {
        if (file[index + shift] == '\"') {
            tokens.emplace_back(type, file.substr(index + 1, shift - 1));
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

#endif

