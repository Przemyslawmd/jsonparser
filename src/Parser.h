
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <vector>

#include "PreToken.h"
#include "TokenValue.h"


class Parser
{
public:
   
    std::unique_ptr<std::map<std::string, TokenValue>> parseInitialTokens(std::vector<PreToken>& initialTokens);
};

#endif

