
#include <map>
#include <memory>
#include <vector>

#include <defines.h>
#include "Token.h"


class Preparser
{
public:

    Preparser();
    std::unique_ptr<std::vector<Token>> parseJSON(const std::string& json);

private:

    void parseNumber();
    void parseString();

    std::map<char, TokenType> tokensMap;
    std::unique_ptr<std::vector<Token>> tokens;
};

