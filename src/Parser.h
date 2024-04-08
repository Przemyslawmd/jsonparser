
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "Token.h"
#include "NodeValue.h"


class Parser
{
    public:
        std::unique_ptr<std::map<std::string, Node>> parseTokens(const std::vector<Token>& tokens);

    private:
        std::stack<std::map<std::string, Node>*> stackObjects;
        std::map<std::string, Node>* currentObject;

        std::stack<std::vector<Node>*> stackArrays;
        std::vector<Node>* currentArray;

        void pushObject(std::string& key);
};

#endif

