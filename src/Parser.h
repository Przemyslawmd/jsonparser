
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "defines.h"
#include "Token.h"
#include "NodeValue.h"


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;


class Parser
{
    public:
        std::unique_ptr<std::map<std::string, Node>> parseTokens(const std::vector<Token>& tokens);

    private:
        std::variant<ObjectNode*, ArrayNode*> currentNode;
        std::stack<std::variant<ObjectNode*, ArrayNode*>> stackNodes;

        State state;

        void pushObjectOnStack(const std::string& key);
        void pushArrayOnStack(const std::string& key);
        
        template<class T> void processData(const std::string& key, const Token& token);
};

#endif

