
#ifndef JSONCPP_PARSER_H
#define JSONCPP_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <vector>

#include "Token.h"
#include "NodeValue.h"


enum class State {
    OBJECT_PARSING,
    ARRAY_PARSING
};


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
        
        void processInteger(const std::string& key, const Token& token);
        void processDouble(const std::string& key, const Token& token);
        void processString(const std::string& key, const Token& token);
        void processBoolean(const std::string& key, const Token& token);
};

#endif

