
#ifndef JX_TOKEN_H
#define JX_TOKEN_H

#include <string>
#include <variant>


using VariantData = std::variant<std::string, int64_t, double, bool, nullptr_t>;

namespace json
{
    enum class TokenType
    {
        CURLY_OPEN,
        CURLY_CLOSE,
        SQUARE_OPEN,
        SQUARE_CLOSE,
        COLON,
        COMMA,
        KEY,
        DATA_STR,
        DATA_BOOL,
        DATA_INT,
        DATA_DOUBLE,
        DATA_NULL,
    };

    struct Token
    {
        TokenType type;
        VariantData data;
    };
}


namespace xml
{
    enum class TokenType
    {
        ANGLE_OPEN,
        ANGLE_CLOSE,
        EQUAL,
        SLASH,
        QUESTION,
        DATA_STR,
        DATA_STR_QUOTA,
        DATA_INT,
        DATA_DOUBLE,
    };

    struct Token
    {
        TokenType type;
        VariantData data;
    };
}

#endif

