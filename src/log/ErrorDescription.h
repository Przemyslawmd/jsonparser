
#ifndef JSONPARSER_ERROR_DESCRIPTION_H
#define JSONPARSER_ERROR_DESCRIPTION_H

#include <unordered_map>
#include <optional>
#include <string>

#include <defines.h>


class ErrorDescription
{
public:
    static const inline std::unordered_map<ErrorCode, std::optional<std::string>> description = 
    {
        { ErrorCode::API_EMPTY,
            "There is no JSON object in JsonApi, parse JSON string or load JSON object" },
        { ErrorCode::API_NOT_EMPTY,
            "Action requires JsonApi instance without JSON object, invoke clear function" },
        { ErrorCode::API_NOT_KEY_IN_MAP,
            "There is no provided key in object" },
        { ErrorCode::API_NOT_KEY_IN_INTERNAL_MAP,
            "Internal error : Key exists in the key mapper but not inside an internal object" },
        { ErrorCode::API_INCONSISTENT_DATA,
            "Using key for array or index for object" },
        { ErrorCode::API_INDEX_OUT_OF_ARRAY,
            "There is no provided index in array" },
        { ErrorCode::API_NODE_NOT_ARRAY,
            "ArrayNode is expected, but Node is not array" },
        { ErrorCode::API_NODE_NOT_OBJECT, 
            "ObjectNode is expected, but Node is not object" },
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
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_BOOL,         std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DOUBLE,       std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_INT,          std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_CLOSE, std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_OPEN,  std::nullopt },
        { ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING,       std::nullopt },
        { ErrorCode::WRITER_NOT_KEY_IN_MAP, 
            "Writer: There is no key id in object" },
    };
};

#endif

