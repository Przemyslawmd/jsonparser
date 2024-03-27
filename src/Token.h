
#ifndef JSONCPP_TOKEN_H
#define JSONCPP_TOKEN_H

#include <string>
#include <variant>

#include <defines.h>


class Token
{
public:

    TokenType type;
    std::variant<std::string, int, double, bool, nullptr_t> data;
};

#endif
