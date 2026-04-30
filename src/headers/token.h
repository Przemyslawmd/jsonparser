
#ifndef JSONPARSER_TOKEN_H
#define JSONPARSER_TOKEN_H

#include <string>
#include <variant>


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


enum class TokenTypeXML
{
    TAG_OPEN,
    TAG_CLOSE,

    EQUAL,
    SLASH,
    QUESTION,

    DATA_STR,
    DATA_INT,
    DATA_DOUBLE,
};


struct Token
{
    TokenType type;
    std::variant<std::string, int64_t, double, bool, nullptr_t> data;
};


struct TokenXML
{
    TokenTypeXML type;
    std::variant<std::string, int64_t, double, bool, nullptr_t> data;
};

#endif
