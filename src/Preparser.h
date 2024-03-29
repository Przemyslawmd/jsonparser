
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
    size_t parseNumber(const std::string& json, size_t index);
    size_t parseString(const std::string& json, size_t index);

    bool checkQuotation(const std::string& json);

    std::map<char, TokenType> tokensMap;
    std::unique_ptr<std::vector<Token>> tokens;

    ParseError error;
};

#endif
