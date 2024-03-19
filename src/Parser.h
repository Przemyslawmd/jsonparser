
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <vector>

#include "PreToken.h"
#include "NodeValue.h"


class Parser
{
public:
   
    std::unique_ptr<std::map<std::string, NodeValue>> parseInitialTokens(std::vector<PreToken>& initialTokens);
};

#endif

