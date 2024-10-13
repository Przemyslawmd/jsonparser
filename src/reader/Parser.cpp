
#include "Parser.h"

#include <ranges>
#include <stack>


std::unique_ptr<ObjectNode> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    bool result;
    auto nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushDataOnStack(nodes.get(), State::OBJECT_PARSING);

    for (const auto& token : tokens | std::views::take(tokens.size() - 1) | std::views::drop(1)) {
        result = true;
        switch (token.type) {
            case TokenType::CURLY_CLOSE:
            case TokenType::SQUARE_CLOSE:
                popDataFromStack();
                break;
            case TokenType::KEY:
                key = std::get<std::string>(token.data);
                break;
            case TokenType::DATA_INT:
                result = processData<int64_t>(key, token);
                break;
            case TokenType::DATA_DOUBLE:
                result = processData<double>(key, token);
                break;
            case TokenType::DATA_STR:
                result = processData<std::string>(key, token);
                break;
            case TokenType::DATA_BOOL:
                result = processData<bool>(key, token);
                break;
            case TokenType::DATA_NULL:
                result = processData<nullptr_t>(key, token);
                break;
            case TokenType::CURLY_OPEN:
                result = pushComplexNodeOnStack<ObjectNode>(key, State::OBJECT_PARSING);
                break;
            case TokenType::SQUARE_OPEN:
                result = pushComplexNodeOnStack<ArrayNode>(key, State::ARRAY_PARSING);
                break;
        }
        if (result == false) {
            return nullptr;
        }
    }
    return nodes;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

template <typename T>
bool Parser::pushComplexNodeOnStack(const std::string& keyStr, State state)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        ObjectNode* objectNode = std::get<ObjectNode*>(nodeStack.top());

        auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        if (optKeyID == std::nullopt) {
            return false;
        }
        uint32_t keyID = optKeyID.value();

        objectNode->emplace(std::make_pair(keyID, T()));
        auto* currentNode = &(std::get<T>(objectNode->at(keyID).value));
        pushDataOnStack(currentNode, state);
    }
    else {
        ArrayNode* arrayNode = std::get<ArrayNode*>(nodeStack.top());
        arrayNode->emplace_back(T());
        auto* currentNode = &(std::get<T>(arrayNode->back().value));
        pushDataOnStack(currentNode, state);
    }
    return true;
}


template <typename T>
bool Parser::processData(const std::string& keyStr, const Token& token)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        if (optKeyID == std::nullopt) {
            return false;
        }
        uint32_t keyID = optKeyID.value();
        ObjectNode* objectNode = std::get<ObjectNode*>(nodeStack.top());
        objectNode->emplace(keyID, std::get<T>(token.data));
    }
    else {
        ArrayNode* arrayNode = std::get<ArrayNode*>(nodeStack.top());
        arrayNode->emplace_back(std::get<T>(token.data));
    }
    return true;
}


void Parser::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
    if (state == State::OBJECT_PARSING) {
        maxMapId += (1 << 16);
        mapIDStack.push(maxMapId);
    }
}


void Parser::popDataFromStack()
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        mapIDStack.pop();
    }
    nodeStack.pop();
    stateStack.pop();
}

