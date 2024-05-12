
#ifndef JSONPARSER_PARSER_H
#define JSONPARSER_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "defines.h"
#include "Token.h"
#include <NodeValue.h>


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;


class Parser
{
    public:
        Parser() = default;

        std::unique_ptr<ObjectNode> parseTokens(const std::vector<Token>& tokens);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::stack<State> stateStack;

        void pushObjectOnStack(const std::string& key);
        void pushArrayOnStack(const std::string& key);

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack, State state);
        void popDataFromStack();

        template<class T> void processData(const std::string& key, const Token& token);
};

#endif

