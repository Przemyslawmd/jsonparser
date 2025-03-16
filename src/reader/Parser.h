
#ifndef JSONPARSER_PARSER_H
#define JSONPARSER_PARSER_H

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
concept ComplexLimit = std::is_same<T, ObjectNode>::value || std::is_same<T, ArrayNode>::value;


template <typename T>
concept PrimitiveLimit = std::is_same<T, std::string>::value || 
                         std::is_same<T, double>::value || 
                         std::is_same<T, int64_t>::value || 
                         std::is_same<T, bool>::value || 
                         std::is_same<T, nullptr_t>::value;


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

        template <typename T> requires ComplexLimit<T>
        bool pushComplexNodeOnStack(const std::string& key, State);

        template <typename T> requires PrimitiveLimit<T>
        bool processData(const std::string& key, const Token&);
};

#endif

