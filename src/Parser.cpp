
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, Node>> Parser::parseTokens(const std::vector<Token>& tokens)
{
    state = State::OBJECT_PARSING;
    bool isKeyParsing = true;
    std::string key;

    auto nodes = std::make_unique<std::map<std::string, Node>>();
    currentNode = nodes.get();

    for (const auto& token : tokens | std::views::drop(1)) {
        if (token.type == TokenType::CURLY_CLOSE || token.type == TokenType::SQUARE_CLOSE) {
            if (stackNodes.empty() == false) {
                currentNode = stackNodes.top();
                stackNodes.pop();
            }
            if (token.type == TokenType::SQUARE_CLOSE) {
                isKeyParsing = true;
            }
            continue;
        }
        
        if (token.type == TokenType::COLON || token.type == TokenType::COMMA) {
            continue;
        }

        if (isKeyParsing) {
            key = std::get<std::string>(token.data);
            isKeyParsing = false;
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
        if (state == State::OBJECT_PARSING) {
            isKeyParsing = true;
        }
    }
    return nodes;
}


void Parser::pushObjectOnStack(const std::string& key)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, ObjectNode()));
        stackNodes.push(currentNode);
        currentNode = &(std::get<ObjectNode>(std::get<ObjectNode*>(currentNode)->at(key).value));
    }
    state = State::OBJECT_PARSING;
}


void Parser::pushArrayOnStack(const std::string& key)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, ArrayNode()));
        stackNodes.push(currentNode);
        currentNode = &(std::get<ArrayNode>(std::get<ObjectNode*>(currentNode)->at(key).value));
    }
    state = State::ARRAY_PARSING;
}


template<class T>
void Parser::processData(const std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(currentNode)->emplace_back(std::get<T>(token.data));
    }
}

