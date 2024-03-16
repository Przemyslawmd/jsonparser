
#include <vector>

#include "defines.h"
#include "PreToken.h"


class Validator
{
    public:

        ParseError validate(std::vector<PreToken>* tokens);

    private:

        ParseError validateBrackets(std::vector<PreToken>* tokens);
};

