
#ifndef JSONCPP_TOKEN_TYPE_H
#define JSONCPP_TOKEN_TYPE_H


enum class TokenType
{
    CURLY_OPEN,
    CURLY_CLOSE,

    SQUARE_OPEN,
    SQUARE_CLOSE,

    COLON,
    COMMA,

    DATA_STR,
    DATA_BOOL,
    DATA_INT,
    DATA_FLOAT,
};

#endif

