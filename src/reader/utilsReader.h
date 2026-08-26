
#pragma once

#include <string>

#include "errorCode.h"
#include "token.h"
#include "log/ErrorStorage.h"


static unsigned int parseString(const std::string& file, unsigned int index)
{
    const size_t pos = file.find('\"', index + 1);
    if (pos == std::string::npos) {
        ErrorStorage::putError(ErrorCode::PREPARSER_STRING_ERROR);
        return 0;
    }
    return pos - index;
}


static std::tuple<size_t, std::variant<int64_t, double>> parseNumber(const std::string& json, unsigned int index)
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
    unsigned int divider = 1;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + (json[index] - '0');
        index++;
        divider *= 10;
    }
    double numberDouble = static_cast<double>(number) / divider;
    return std::make_tuple(index - 1, isMinus ? numberDouble * -1.0 : numberDouble);
}

