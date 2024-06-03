
#include <map>
#include <optional>
#include <string>

#include "../headers/defines.h"


class Message
{
public:
    std::string getMessage(Result result);

private:
    const std::map<Result, std::optional<std::string>> messages = 
    {
        { Result::API_EMPTY,
            "There is no JSON object in JsonApi, parse JSON string or load JSON object" },
        { Result::API_NOT_EMPTY,
            "Action requires JsonApi instance without JSON object, invoke clear function" },
        { Result::API_NOT_KEY_IN_MAP,
            "There is no provided key in object" },
        { Result::API_INCONSISTENT_DATA,
            "Using key for array or index for object" },
        { Result::API_INDEX_OUT_OF_ARRAY,
            "There is no provided index in array" },
        { Result::API_NODE_NOT_ARRAY,
            "Array is expected, but Node is not array" },
        { Result::API_NODE_NOT_OBJECT, 
            "Object is expected, but Node is not object" },
        { Result::PREPARSER_STRING_ERROR,                      std::nullopt },
        { Result::PREPARSER_UNKNOWN_SYMBOL,                    std::nullopt },
        { Result::VALIDATOR_BRACKET_CURLY_ERROR,               std::nullopt },
        { Result::VALIDATOR_BRACKET_SQUARE_ERROR,              std::nullopt },
        { Result::VALIDATOR_IMPROPER_BEGINNING,                std::nullopt },
        { Result::VALIDATOR_IMPROPER_END,                      std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA,        std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON,        std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE,  std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN,   std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL,    std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE,  std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT,     std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_CLOSE, std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_OPEN,  std::nullopt },
        { Result::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING,       std::nullopt },
    };
};

