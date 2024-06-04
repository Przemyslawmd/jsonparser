
#ifndef JSONPARSER_VALIDATOR_H
#define JSONPARSER_VALIDATOR_H

#include <vector>

#include "defines.h"
#include "Token.h"


class Validator
{
    public:
        ErrorCode validate(const std::vector<Token>& tokens);

    private:
        ErrorCode validateBrackets(const std::vector<Token>& tokens);
        ErrorCode checkRequirements(const std::vector<Token>& tokens);
};

#endif

