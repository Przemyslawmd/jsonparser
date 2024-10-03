
#ifndef JSONPARSER_PARSER_H
#define JSONPARSER_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include "defines.h"
#include "../keyMapper.h"
#include "Token.h"
#include "NodeValue.h"


class Parser
{
    public:
        Parser(KeyMapper& keyMapper) : keyMapper(keyMapper) {};

        std::unique_ptr<ObjectNode> parseTokens(const std::vector<Token>&);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::stack<State> stateStack;
        std::stack<uint32_t> mapIDStack;
        uint32_t maxMapId = 0;

        KeyMapper& keyMapper;

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack, State);
        void popDataFromStack();

        template <typename T>
        bool pushComplexNodeOnStack(const std::string& key, State);

        template <typename T>
        bool processData(const std::string& key, const Token&);
};

#endif

