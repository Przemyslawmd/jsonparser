
#include <iostream>
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, TokenValue>> Parser::parseInitialTokens(std::vector<PreToken>& initialTokens)
{
    bool isKeyParsing = true;
    std::string key;

    auto tokens = std::make_unique<std::map<std::string, TokenValue>>();
    std::map<std::string, TokenValue>* currentMap = tokens.get();
    
    int i = 0;
    for (auto initToken : initialTokens) {
        
        if (i == 0) {
            i = 1;
            continue;
        }

        if (initToken.type == TokenType::CURLY_CLOSE) {
            continue;
        }
        
        if (initToken.type == TokenType::COLON || initToken.type == TokenType::COMMA) {
            continue;
        }
        if (isKeyParsing) {
            key = std::string{ std::get<std::string>(initToken.data) };
            isKeyParsing = false;
            continue;
        }
        if (isKeyParsing == false && initToken.type == TokenType::DATA_INT) {
            TokenValue val;
            val.value = int(std::get<int>(initToken.data));
            currentMap->emplace( std::make_pair(key, val.value));
            isKeyParsing = true;
            continue;
        }
        if (isKeyParsing == false && initToken.type == TokenType::DATA_STR) {
            TokenValue val;
            val.value = std::string{ std::get<std::string>(initToken.data) };
            currentMap->emplace(std::make_pair(key,val));
            isKeyParsing = true;
            continue;
        } 
        if (isKeyParsing == false && initToken.type == TokenType::CURLY_OPEN) {
            auto map = std::map<std::string, TokenValue>();
            tokens->emplace(std::make_pair(key, map));
            currentMap = &(std::get<std::map<std::string, TokenValue>>(currentMap->at(key).value));
            isKeyParsing = true;
            continue;
        }
    }
        
    return tokens;
}

