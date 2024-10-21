
#ifndef JSONPARSER_ERROR_DESCRIPTION_H
#define JSONPARSER_ERROR_DESCRIPTION_H

#include <unordered_map>
#include <optional>
#include <string>

#include "Error.h"


static const std::unordered_map<ErrorCode, std::optional<std::string>> ErrorDetails = 
{
    { ErrorCode::EXCEPTION_MANAGER_NOT_KEY_IN_OBJECT,
        "Exception: Key exists in the key mapper but not in the object" },

    { ErrorCode::KEY_MAPPER_KEY_STR_REPEAT,
        "KeyMapper: Key in JSON repeated" },

    { ErrorCode::MANAGER_EMPTY,
        "There is no JSON object, parse JSON string or load JSON object" },
    { ErrorCode::MANAGER_NOT_KEY_IN_OBJECT,
        "There is no provided key in object" },
    { ErrorCode::MANAGER_IMPROPER_PATH,
        "Using key for array or index for object" },
    { ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY,
        "Provided index is out of array" },
    { ErrorCode::MANAGER_NODE_NOT_ARRAY,
        "Array node is expected" },
    { ErrorCode::MANAGER_NODE_NOT_OBJECT,
        "Object node is expected" },
    { ErrorCode::MANAGER_ROOT_NOT_EMPTY,
        "Action requires no JSON object, invoke clear function" },
    { ErrorCode::MANAGER_ROOT_NOT_OBJECT,            std::nullopt },
    { ErrorCode::PREPARSER_STRING_ERROR,             std::nullopt },
    { ErrorCode::PREPARSER_UNKNOWN_SYMBOL,           std::nullopt },
    { ErrorCode::VALIDATOR_BRACKET_CURLY,            std::nullopt },
    { ErrorCode::VALIDATOR_BRACKET_SQUARE,           std::nullopt },
    { ErrorCode::VALIDATOR_IMPROPER_BEGIN,           std::nullopt },
    { ErrorCode::VALIDATOR_IMPROPER_END,             std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_COMMA,        std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_COLON,        std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_CLOSE,  std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_OPEN,   std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_BOOL,         std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_DOUBLE,       std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_INT,          std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_SQUARE_CLOSE, std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_SQUARE_OPEN,  std::nullopt },
    { ErrorCode::VALIDATOR_TOKEN_AFTER_STRING,       std::nullopt },
};

#endif

