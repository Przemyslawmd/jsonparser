
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
    size_t map;
    size_t node;
};


class Parser
{
    public:
        Parser(KeyMapper& keyMapper) : keyMapper(keyMapper) {};

        std::unique_ptr<Object> parseTokens(const std::vector<Token>&);

    private:
        std::stack<std::variant<Object*, Array*>> nodeStack;
        std::stack<State> stateStack;
        std::stack<ItemId> idStack;
        size_t maxMapId = 0;

        KeyMapper& keyMapper;

        void pushDataOnStack(std::variant<Object*, Array*> nodeStack, State);
        void popDataFromStack();

        template <typename T>
        void pushInnerNodeOnStack(const std::string& key, State);

        template <typename T>
        void processData(const std::string& key, const Token&);

        size_t createId();
};

#endif

