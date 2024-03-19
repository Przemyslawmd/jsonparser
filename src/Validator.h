
#include <vector>

#include "defines.h"
#include "Token.h"


class Validator
{
    public:

        ParseError validate(std::vector<Token>* tokens);

    private:

        ParseError validateBrackets(std::vector<Token>* tokens);
};

