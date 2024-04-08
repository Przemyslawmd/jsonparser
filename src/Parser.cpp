
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
            processInteger(key, token);
        }
        else if (token.type == TokenType::DATA_DOUBLE) {
            processDouble(key, token);
        }
        else if (token.type == TokenType::DATA_STR) {
            processString(key, token);
        }
        else if (token.type == TokenType::DATA_BOOL) {
            processBoolean(key, token);
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


void Parser::processInteger(const std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<int>(token.data)));
    }
}


void Parser::processDouble(const std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<double>(token.data)));
    }

}


void Parser::processString(const std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<std::string>(token.data)));
    }
    else {
        std::get<ArrayNode*>(currentNode)->emplace_back(std::get<std::string>(token.data));
    }
}


void Parser::processBoolean(const std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<bool>(token.data)));
    }
}

