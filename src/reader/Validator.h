
#ifndef JSONPARSER_VALIDATOR_H
#define JSONPARSER_VALIDATOR_H

#include <vector>

#include "token.h"
#include "error.h"


class Validator
{
    public:
        bool validate(const std::vector<Token>& tokens);

    private:
        ErrorCode validateBrackets(const std::vector<Token>& tokens);
        bool checkTokensSequence(const std::vector<Token>& tokens);

        void createError(ErrorCode, TokenType first, TokenType second);
};

#endif

