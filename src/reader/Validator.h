
#ifndef JSONPARSER_VALIDATOR_H
#define JSONPARSER_VALIDATOR_H

#include <vector>

#include "defines.h"
#include "Token.h"


class Validator
{
    public:
        Result validate(const std::vector<Token>& tokens);

    private:
        Result validateBrackets(const std::vector<Token>& tokens);
        Result checkRequirements(const std::vector<Token>& tokens);
};

#endif

