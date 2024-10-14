

#ifndef JSONPARSER_TOKEN_DESCRIPTION_H
#define JSONPARSER_TOKEN_DESCRIPTION_H

#include <unordered_map>
#include <string>

#include "defines.h"


static const std::unordered_map<TokenType, std::string> TokenDesc =
{
    { TokenType::CURLY_OPEN,   "Opening curly bracket" },
    { TokenType::CURLY_CLOSE,  "Closing curly bracket" },
    { TokenType::SQUARE_OPEN,  "Opening square bracket" },
    { TokenType::SQUARE_CLOSE, "Closing square bracket" },
    { TokenType::COLON,        "Colon" },
    { TokenType::COMMA,        "Comma" },
    { TokenType::DATA_STR,     "String" },
    { TokenType::KEY,          "String" },
    { TokenType::DATA_BOOL,    "Boolean" },
    { TokenType::DATA_INT,     "Integer number" },
    { TokenType::DATA_DOUBLE,  "Double number" },
    { TokenType::DATA_NULL,    "Null" },
};

#endif

