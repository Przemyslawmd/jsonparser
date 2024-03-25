
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <vector>

#include "Token.h"
#include "NodeValue.h"


class Parser
{
public:
   
    std::unique_ptr<std::map<std::string, Node>> parseTokens(std::vector<Token>& tokens);
};

#endif

