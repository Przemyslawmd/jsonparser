
#pragma once

#include <string>

#include "errorCode.h"
#include "log/ErrorStorage.h"
#include "token.h"


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


static std::tuple<size_t, std::variant<int64_t, double>> parseNumber(const std::string& json, size_t index)
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
        return std::make_tuple(index - 1, isMinus ? number * -1 : number);
    }

    index++;
    size_t divider = 1;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + (json[index] - '0');
        index++;
        divider *= 10;
    }
    double numberDouble = (double) number / divider;
    return std::make_tuple(index - 1, isMinus ? numberDouble * -1.0 : numberDouble);
}

