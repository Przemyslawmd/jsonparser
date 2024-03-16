
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


enum class ParseError
{
    NOT_ERROR,

    FIRST_CHAR_NOT_CURLY_OPEN,
    LAST_CHAR_NOT_CURLY_CLOSE,

    STRING_NOT_ENDED,
};

#endif

