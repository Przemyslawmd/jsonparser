
#pragma once

#include <unordered_map>
#include <optional>
#include <string>

#include "errorCode.h"


static const std::unordered_map<ErrorCode, std::optional<std::string>> ErrorDetails = 
{
    { ErrorCode::EXCEPTION_MANAGER_NOT_KEY_IN_OBJECT,
        "Exception: Key exists in the key mapper but not in the object" },

    { ErrorCode::KEY_MAPPER_KEY_STR_REPEAT,
        "KeyMapper: Key in JSON repeated" },

    { ErrorCode::MANAGER__EMPTY,
        "There is no object, parse string or load an object" },
    { ErrorCode::MANAGER__NOT_KEY_IN_OBJECT,
        "There is no provided key in object" },
    { ErrorCode::MANAGER__IMPROPER_PATH,
        "Using key for array or index for object" },
    { ErrorCode::MANAGER__INDEX_OUT_OF_ARRAY,
        "Provided index is out of array" },
    { ErrorCode::MANAGER__NODE_NOT_ARRAY,
        "Array node is expected" },
    { ErrorCode::MANAGER__NODE_NOT_OBJECT,
        "Object node is expected" },
    { ErrorCode::MANAGER__ROOT_NOT_EMPTY,
        "Action requires no object, invoke clear function" },
    { ErrorCode::MANAGER__ROOT_NOT_OBJECT,            std::nullopt },
    { ErrorCode::PREPARSER_STRING_ERROR,             std::nullopt },
    { ErrorCode::JSON_PREPARSER_UNKNOWN_SYMBOL,      std::nullopt },
    { ErrorCode::JSON_VALIDATOR__BRACKET_CURLY,       std::nullopt },
    { ErrorCode::JSON_VALIDATOR__BRACKET_SQUARE,      std::nullopt },
    { ErrorCode::JSON_VALIDATOR__IMPROPER_BEGIN,      std::nullopt },
    { ErrorCode::JSON_VALIDATOR__IMPROPER_END,        std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_COMMA,         std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_COLON,         std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_CURLY_CLOSE,   std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_CURLY_OPEN,    std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_BOOL,          std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_DOUBLE,        std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_INT,           std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_SQUARE_CLOSE,  std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_SQUARE_OPEN,   std::nullopt },
    { ErrorCode::JSON_VALIDATOR__AFTER_STRING,        std::nullopt },
};


