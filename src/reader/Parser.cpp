
#include "Parser.h"

#include <stack>
#include <variant>


std::unique_ptr<ObjectNode> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    auto nodes = std::make_unique<ObjectNode>();
    pushDataOnStack(nodes.get(), State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {
        if (it->type == TokenType::CURLY_CLOSE || it->type == TokenType::SQUARE_CLOSE) {
            popDataFromStack();
        }
        else if (it->type == TokenType::KEY) {
            key = std::get<std::string>(it->data);
        }
        else if (it->type == TokenType::DATA_INT) {
            processData<int>(key, *it);
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
        else if (it->type == TokenType::CURLY_OPEN) {
            pushObjectOnStack(key);
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            pushArrayOnStack(key);
        }
    }
    return nodes;
}


void Parser::pushObjectOnStack(const std::string& key)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(nodeStack.top())->emplace(std::make_pair(key, ObjectNode()));
        auto currentNode = &(std::get<ObjectNode>(std::get<ObjectNode*>(nodeStack.top())->at(key).value));
        pushDataOnStack(currentNode, State::OBJECT_PARSING);
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(ObjectNode());
        auto currentNode = &(std::get<ObjectNode>(std::get<ArrayNode*>(nodeStack.top())->back().value));
        pushDataOnStack(currentNode, State::OBJECT_PARSING);
    }
}


void Parser::pushArrayOnStack(const std::string& key)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(nodeStack.top())->emplace(std::make_pair(key, ArrayNode()));
        auto currentNode = &(std::get<ArrayNode>(std::get<ObjectNode*>(nodeStack.top())->at(key).value));
        pushDataOnStack(currentNode, State::ARRAY_PARSING);
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(ArrayNode());
        auto currentNode = &(std::get<ArrayNode>(std::get<ArrayNode*>(nodeStack.top())->back().value));
        pushDataOnStack(currentNode, State::ARRAY_PARSING);
    }
}


template<class T>
void Parser::processData(const std::string& key, const Token& token)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(nodeStack.top())->emplace(std::make_pair(key, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(std::get<T>(token.data));
    }
}


void Parser::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
}


void Parser::popDataFromStack()
{
    nodeStack.pop();
    stateStack.pop();
}

