
#ifndef READER_JSON_VALIDATOR_H
#define READER_JSON_VALIDATOR_H

#include <vector>

#include "token.h"
#include "errorCode.h"


class Validator
{
    public:
        bool validate(const std::vector<json::Token>& tokens);

    private:
        ErrorCode validateBrackets(const std::vector<json::Token>& tokens);
        bool checkTokensSequence(const std::vector<json::Token>& tokens);

        void createError(ErrorCode, TokenType first, TokenType second);
};

#endif

