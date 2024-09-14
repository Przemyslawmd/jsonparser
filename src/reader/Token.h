
#ifndef JSONPARSER_TOKEN_H
#define JSONPARSER_TOKEN_H

#include <string>
#include <variant>

#include "defines.h"


class Token
{
public:

    TokenType type;
    std::variant<std::string, int64_t, double, bool, nullptr_t> data;
};

#endif
