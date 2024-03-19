
#ifndef JSONCPP_PREPARSER_H
#define JSONCPP_PREPARSER_H

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

    ParseError getError();

private:
    int parseNumber(const std::string& json, int index);
    int parseString(const std::string& json, int index);

    std::map<char, TokenType> tokensMap;
    std::unique_ptr<std::vector<Token>> tokens;

    ParseError error;
};

#endif
