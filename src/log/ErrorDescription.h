
#ifndef JSONPARSER_ERROR_MESSAGE_H
#define JSONPARSER_ERROR_MESSAGE_H

#include <map>
#include <optional>
#include <string>

#include "../headers/defines.h"


class ErrorDescription
{
public:
    static const inline std::map<ErrorCode, std::optional<std::string>> description = 
    {
        { ErrorCode::API_EMPTY,
            "There is no JSON object in JsonApi, parse JSON string or load JSON object" },
        { ErrorCode::API_NOT_EMPTY,
            "Action requires JsonApi instance without JSON object, invoke clear function" },
        { ErrorCode::API_NOT_KEY_IN_MAP,
            "There is no provided key in object" },
        { ErrorCode::API_INCONSISTENT_DATA,
            "Using key for array or index for object" },
        { ErrorCode::API_INDEX_OUT_OF_ARRAY,
            "There is no provided index in array" },
        { ErrorCode::API_NODE_NOT_ARRAY,
            "Array is expected, but Node is not array" },
        { ErrorCode::API_NODE_NOT_OBJECT, 
            "Object is expected, but Node is not object" },
        { ErrorCode::PREPARSER_STRING_ERROR,                      std::nullopt },
        { ErrorCode::PREPARSER_UNKNOWN_SYMBOL,                    std::nullopt },
        { ErrorCode::VALIDATOR_BRACKET_CURLY_ERROR,               std::nullopt },
        { ErrorCode::VALIDATOR_BRACKET_SQUARE_ERROR,              std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_BEGINNING,                std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_END,                      std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA,        std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON,        std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE,  std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN,   std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL,    std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE,  std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT,     std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_CLOSE, std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_OPEN,  std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING,       std::nullopt },
    };
};

#endif

