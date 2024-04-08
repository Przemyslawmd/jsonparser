
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
        if (token.type == TokenType::CURLY_CLOSE) {
            if (stackNodes.empty() == false) {
                currentNode = stackNodes.top();
                stackNodes.pop();
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
        isKeyParsing = true;
    }
    return nodes;
}


void Parser::pushObjectOnStack(std::string& key)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectPointer>(currentNode)->emplace(std::make_pair(key, std::map<std::string, Node>()));
        stackNodes.push(currentNode);
        currentNode = &(std::get<std::map<std::string, Node>>(std::get<ObjectPointer>(currentNode)->at(key).value));
    }
}


void Parser::processInteger(std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectPointer>(currentNode)->emplace(std::make_pair(key, std::get<int>(token.data)));
    }
}


void Parser::processDouble(std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectPointer>(currentNode)->emplace(std::make_pair(key, std::get<double>(token.data)));
    }

}


void Parser::processString(std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectPointer>(currentNode)->emplace(std::make_pair(key, std::get<std::string>(token.data)));
    }
}


void Parser::processBoolean(std::string& key, const Token& token)
{
    if (state == State::OBJECT_PARSING) {
        std::get<ObjectPointer>(currentNode)->emplace(std::make_pair(key, std::get<bool>(token.data)));
    }
}

