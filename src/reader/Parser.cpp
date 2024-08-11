
#include "Parser.h"

#include <stack>


std::unique_ptr<ObjectNode> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    auto nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushDataOnStack(nodes.get(), State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {
        if (it->type == TokenType::CURLY_CLOSE || it->type == TokenType::SQUARE_CLOSE) {
            popDataFromStack();
        }
        else if (it->type == TokenType::KEY) {
            key = std::get<std::string>(it->data);
        }
        else if (it->type == TokenType::DATA_INT) {
            processData<int64_t>(key, *it);
        }
        else if (it->type == TokenType::DATA_DOUBLE) {
            processData<double>(key, *it);
        }
        else if (it->type == TokenType::DATA_STR) {
            processData<std::string>(key, *it);
        }
        else if (it->type == TokenType::DATA_BOOL) {
            processData<bool>(key, *it);
        }
        else if (it->type == TokenType::DATA_NULL) {
            processData<nullptr_t>(key, *it);
        }
        else if (it->type == TokenType::CURLY_OPEN) {
            pushInnerNodeOnStack<ObjectNode>(key, State::OBJECT_PARSING);
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            pushInnerNodeOnStack<ArrayNode>(key, State::ARRAY_PARSING);
        }
    }
    return nodes;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

template <typename T>
void Parser::pushInnerNodeOnStack(const std::string& keyStr, State state)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());
        uint32_t keyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        obj->emplace(std::make_pair(keyID, T()));
        auto* currentNode = &(std::get<T>(obj->at(keyID).value));
        pushDataOnStack(currentNode, state);
    }
    else {
        ArrayNode* arr = std::get<ArrayNode*>(nodeStack.top());
        arr->emplace_back(T());
        auto* currentNode = &(std::get<T>(arr->back().value));
        pushDataOnStack(currentNode, state);
    }
}


template <typename T>
void Parser::processData(const std::string& keyStr, const Token& token)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        uint32_t keyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        std::get<ObjectNode*>(nodeStack.top())->emplace(std::make_pair(keyID, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(std::get<T>(token.data));
    }
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

