
#ifndef JSONCPP_PRETOKEN_H
#define JSONCPP_PRETOKEN_H

#include <string>
#include <variant>

#include <defines.h>


class PreToken
{
public:

    TokenType type;
    std::variant<std::string, int, nullptr_t> data;
};

#endif
