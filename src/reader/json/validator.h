
#ifndef JX_READER_JSON_VALIDATOR_H
#define JX_READER_JSON_VALIDATOR_H

#include <vector>

#include "token.h"
#include "errorCode.h"


namespace json
{
class Validator
{
    public:
        bool validate(const std::vector<Token>& tokens);

    private:
        ErrorCode validateBrackets(const std::vector<Token>& tokens);
        bool checkTokensSequence(const std::vector<Token>& tokens);

        void createError(ErrorCode, TokenType first, TokenType second);
};
}

#endif

