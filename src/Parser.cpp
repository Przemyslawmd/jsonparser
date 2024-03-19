
#include <iostream>
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, NodeValue>> Parser::parseInitialTokens(std::vector<Token>& initialTokens)
{
    bool isKeyParsing = true;
    std::string key;

    auto tokens = std::make_unique<std::map<std::string, NodeValue>>();
    std::map<std::string, NodeValue>* currentNode = tokens.get();
    
    for (auto initToken : initialTokens | std::views::drop(1)) {
        
        if (initToken.type == TokenType::CURLY_CLOSE) {
            continue;
        }
        
        if (initToken.type == TokenType::COLON || initToken.type == TokenType::COMMA) {
            continue;
        }
        if (isKeyParsing) {
            key = std::get<std::string>(initToken.data);
            isKeyParsing = false;
            continue;
        }
        if (isKeyParsing == false && initToken.type == TokenType::DATA_INT) {
            currentNode->emplace(std::make_pair(key, std::get<int>(initToken.data)));
            isKeyParsing = true;
            continue;
        }
        if (isKeyParsing == false && initToken.type == TokenType::DATA_STR) {
            currentNode->emplace(std::make_pair(key, std::get<std::string>(initToken.data)));
            isKeyParsing = true;
            continue;
        } 
        if (isKeyParsing == false && initToken.type == TokenType::CURLY_OPEN) {
            auto map = std::map<std::string, NodeValue>();
            tokens->emplace(std::make_pair(key, map));
            currentNode = &(std::get<std::map<std::string, NodeValue>>(currentNode->at(key).value));
            isKeyParsing = true;
            continue;
        }
    }
    return tokens;
}

