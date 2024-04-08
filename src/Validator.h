
#include <vector>

#include "defines.h"
#include "Token.h"


class Validator
{
    public:
        ParseError validate(const std::vector<Token>& tokens);

    private:
        ParseError validateBrackets(const std::vector<Token>& tokens);
        ParseError checkRequirements(const std::vector<Token>& tokens);
};

