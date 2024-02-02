
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

    int parseNumber(const std::string& json, int index);
    int parseString(const std::string& json, int index);

    std::map<char, TokenType> tokensMap;
    std::unique_ptr<std::vector<Token>> tokens;
};

