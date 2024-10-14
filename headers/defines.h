
#ifndef JSONCPP_DEFINES_H
#define JSONCPP_DEFINES_H


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


enum class State
{
    OBJECT_PARSING,
    ARRAY_PARSING
};

#endif

