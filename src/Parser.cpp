
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, Node>> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    auto nodes = std::make_unique<std::map<std::string, Node>>();
    currentNode = nodes.get();

    states.push(State::OBJECT_PARSING);
    for (const auto& token : tokens | std::views::drop(1)) {
        if (token.type == TokenType::CURLY_CLOSE || token.type == TokenType::SQUARE_CLOSE) {
            if (stackNodes.empty() == false) {
                currentNode = stackNodes.top();
                stackNodes.pop();
            }
            states.pop();
            continue;
        }

        if (token.type == TokenType::COLON || token.type == TokenType::COMMA) {
            continue;
        }

        if (token.type == TokenType::KEY) {
            key = std::get<std::string>(token.data);
            continue;
        }

        if (token.type == TokenType::DATA_INT) {
            processData<int>(key, token);
        }
        else if (token.type == TokenType::DATA_DOUBLE) {
            processData<double>(key, token);
        }
        else if (token.type == TokenType::DATA_STR) {
            processData<std::string>(key, token);
        }
        else if (token.type == TokenType::DATA_BOOL) {
            processData<bool>(key, token);
        }
        else if (token.type == TokenType::CURLY_OPEN) {
            pushObjectOnStack(key);
        }
        else if (token.type == TokenType::SQUARE_OPEN) {
            pushArrayOnStack(key);
        }
    }
    return nodes;
}


void Parser::pushObjectOnStack(const std::string& key)
{
    
    if (states.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, ObjectNode()));
        stackNodes.push(currentNode);
        currentNode = &(std::get<ObjectNode>(std::get<ObjectNode*>(currentNode)->at(key).value));
    }
    else {
        ObjectNode obj;
        std::get<ArrayNode*>(currentNode)->emplace_back(obj);
        stackNodes.push(currentNode);
        currentNode = &(std::get<ObjectNode>(std::get<ArrayNode*>(currentNode)->back().value));
    }
    states.push(State::OBJECT_PARSING);
}


void Parser::pushArrayOnStack(const std::string& key)
{
    if (states.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, ArrayNode()));
        stackNodes.push(currentNode);
        currentNode = &(std::get<ArrayNode>(std::get<ObjectNode*>(currentNode)->at(key).value));
    }
    else {
    }
    states.push(State::ARRAY_PARSING);
}


template<class T>
void Parser::processData(const std::string& key, const Token& token)
{
    if (states.top() == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(currentNode)->emplace_back(std::get<T>(token.data));
    }
}

