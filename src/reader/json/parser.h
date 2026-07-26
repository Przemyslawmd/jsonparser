
#pragma once

#include <concepts>
#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include "keyMapper.h"
#include "node.h"
#include "state.h"
#include "token.h"


template <typename T>
concept ComplexNode = std::same_as<T, ObjectNode> || std::same_as<T, ArrayNode>;

template <typename T>
concept SimpleNode = std::same_as<T, std::string> || 
                     std::same_as<T, double> || 
                     std::same_as<T, int64_t> || 
                     std::same_as<T, bool> || 
                     std::same_as<T, nullptr_t>;


namespace json
{
class Parser
{
    public:
        Parser(KeyMapper& keyMapper) : keyMapper(keyMapper), maxMapId(0) {};

        std::unique_ptr<ObjectNode> parseTokens(const std::vector<Token>&);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::stack<State> stateStack;
        std::stack<uint32_t> mapIDStack;
        uint32_t maxMapId;

        KeyMapper& keyMapper;

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack, State);
        void popDataFromStack();

        template <typename T> requires ComplexNode<T>
        bool pushComplexNodeOnStack(const std::string& key, State);

        template <typename T> requires SimpleNode<T>
        bool processData(const std::string& key, const Token&);
};
}

