
#include <iostream>
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, NodeValue>> Parser::parseInitialTokens(std::vector<Token>& tokens)
{
    bool isKeyParsing = true;
    std::string key;

    auto nodes = std::make_unique<std::map<std::string, NodeValue>>();
    std::map<std::string, NodeValue>* currentNode = nodes.get();
    std::stack<std::map<std::string, NodeValue>*> nodesPointer;

    for (auto token : tokens | std::views::drop(1)) {
        
        if (token.type == TokenType::CURLY_CLOSE) {
            if (nodesPointer.empty() == false) {
                currentNode = nodesPointer.top();
                nodesPointer.pop();
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
        if (isKeyParsing == false && token.type == TokenType::DATA_INT) {
            currentNode->emplace(std::make_pair(key, std::get<int>(token.data)));
            isKeyParsing = true;
            continue;
        }
        if (isKeyParsing == false && token.type == TokenType::DATA_STR) {
            currentNode->emplace(std::make_pair(key, std::get<std::string>(token.data)));
            isKeyParsing = true;
            continue;
        }
        if (isKeyParsing == false && token.type == TokenType::DATA_BOOL) {
            currentNode->emplace(std::make_pair(key, std::get<bool>(token.data)));
            isKeyParsing = true;
            continue;
        }
        if (isKeyParsing == false && token.type == TokenType::CURLY_OPEN) {
            auto map = std::map<std::string, NodeValue>();
            nodes->emplace(std::make_pair(key, map));
            nodesPointer.push(currentNode);
            currentNode = &(std::get<std::map<std::string, NodeValue>>(currentNode->at(key).value));
            isKeyParsing = true;
            continue;
        }
    }
    return nodes;
}

