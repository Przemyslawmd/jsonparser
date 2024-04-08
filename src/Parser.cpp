
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>


std::unique_ptr<std::map<std::string, Node>> Parser::parseTokens(const std::vector<Token>& tokens)
{
    bool isKeyParsing = true;
    std::string key;

    auto nodes = std::make_unique<std::map<std::string, Node>>();
    currentObject = nodes.get();

    for (const auto& token : tokens | std::views::drop(1)) {
        if (token.type == TokenType::CURLY_CLOSE) {
            if (stackObjects.empty() == false) {
                currentObject = stackObjects.top();
                stackObjects.pop();
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
            currentObject->emplace(std::make_pair(key, std::get<int>(token.data)));
        }
        else if (token.type == TokenType::DATA_DOUBLE) {
            currentObject->emplace(std::make_pair(key, std::get<double>(token.data)));
        }
        else if (token.type == TokenType::DATA_STR) {
            currentObject->emplace(std::make_pair(key, std::get<std::string>(token.data)));
        }
        else if (token.type == TokenType::DATA_BOOL) {
            currentObject->emplace(std::make_pair(key, std::get<bool>(token.data)));
        }
        else if (token.type == TokenType::CURLY_OPEN) {
            pushObject(key);
        }
        isKeyParsing = true;
    }
    return nodes;
}


void Parser::pushObject(std::string& key)
{
    currentObject->emplace(std::make_pair(key, std::map<std::string, Node>()));
    stackObjects.push(currentObject);
    currentObject = &(std::get<std::map<std::string, Node>>(currentObject->at(key).value));
}

