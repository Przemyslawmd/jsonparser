
#ifndef JSONCPP_PARSERKEY_H
#define JSONCPP_PARSERKEY_H

#include <memory>
#include <vector>

#include "Token.h"


class ParserKey
{
public:
    ParserKey() = default;

    std::unique_ptr<std::vector<Token>> createKeyTokens(std::unique_ptr<std::vector<Token>>);
};

#endif
