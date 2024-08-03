
#ifndef JSONPARSER_PARSER_H
#define JSONPARSER_PARSER_H

#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include <defines.h>
#include "../keyMapper.h"
#include "Token.h"
#include <NodeValue.h>


struct ItemId {
    uint32_t map;
    uint32_t node;
};


class Parser
{
    public:
        Parser(KeyMapper& keyMapper) : keyMapper(keyMapper) {};

        std::unique_ptr<ObjectNode> parseTokens(const std::vector<Token>&);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::stack<State> stateStack;
        std::stack<ItemId> idStack;
        uint32_t maxMapId = 0;

        KeyMapper& keyMapper;

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack, State);
        void popDataFromStack();

        template <typename T>
        void pushInnerNodeOnStack(const std::string& key, State);

        template <typename T>
        void processData(const std::string& key, const Token&);

        uint32_t createId();
};

#endif

