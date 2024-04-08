
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


using ObjectPointer = std::map<std::string, Node>*;
using ArrayPointer = std::vector<Node>*;


class Parser
{
    public:
        std::unique_ptr<std::map<std::string, Node>> parseTokens(const std::vector<Token>& tokens);

    private:
        std::variant<ObjectPointer, ArrayPointer> currentNode;
        std::stack<std::variant<ObjectPointer, ArrayPointer>> stackNodes;

        State state;

        void pushObjectOnStack(std::string& key);
        
        void processInteger(std::string& key, const Token& token);
        void processDouble(std::string& key, const Token& token);
        void processString(std::string& key, const Token& token);
        void processBoolean(std::string& key, const Token& token);
};

#endif

