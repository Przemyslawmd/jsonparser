
#ifndef JSONPARSER_ERROR_H
#define JSONPARSER_ERROR_H

#include <optional>
#include <string>


enum class ErrorCode
{
    NO_ERROR,

    EXCEPTION_MANAGER_NOT_KEY_IN_OBJECT,

    KEY_MAPPER_KEY_STR_REPEAT,

    MANAGER_ROOT_NOT_EMPTY,
    MANAGER_ROOT_NOT_OBJECT,
    MANAGER_EMPTY,
    MANAGER_NOT_KEY_IN_OBJECT,
    MANAGER_IMPROPER_PATH,
    MANAGER_INDEX_OUT_OF_ARRAY,
    MANAGER_NODE_NOT_ARRAY,
    MANAGER_NODE_NOT_OBJECT,

    PREPARSER_STRING_ERROR,
    PREPARSER_UNKNOWN_SYMBOL,

    VALIDATOR_BRACKET_CURLY,
    VALIDATOR_BRACKET_SQUARE,

    VALIDATOR_IMPROPER_BEGIN,
    VALIDATOR_IMPROPER_END,

    VALIDATOR_TOKEN_AFTER_COMMA,
    VALIDATOR_TOKEN_AFTER_COLON,
    VALIDATOR_TOKEN_AFTER_CURLY_CLOSE,
    VALIDATOR_TOKEN_AFTER_CURLY_OPEN,
    VALIDATOR_TOKEN_AFTER_BOOL,
    VALIDATOR_TOKEN_AFTER_DOUBLE,
    VALIDATOR_TOKEN_AFTER_INT,
    VALIDATOR_TOKEN_AFTER_SQUARE_CLOSE,
    VALIDATOR_TOKEN_AFTER_SQUARE_OPEN,
    VALIDATOR_TOKEN_AFTER_STRING,

    WRITER_NOT_KEY_IN_MAP
};


class Error
{
public:
    Error(ErrorCode, std::optional<std::string> info = std::nullopt);

    ErrorCode getCode() const;
    std::string getDetails() const;

private:
    std::optional<std::string> details;
    ErrorCode errorCode;
};

#endif

