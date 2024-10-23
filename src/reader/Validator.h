
#ifndef JSONPARSER_VALIDATOR_H
#define JSONPARSER_VALIDATOR_H

#include <memory>
#include <vector>

#include "token.h"
#include "error.h"


class Validator
{
    public:
        bool validate(const std::vector<Token>& tokens);

    private:
        bool validateBrackets(const std::vector<Token>& tokens);
        bool checkTokensSequence(const std::vector<Token>& tokens);

        void createTypeAfterError(ErrorCode, TokenType first, TokenType second);
};

#endif

