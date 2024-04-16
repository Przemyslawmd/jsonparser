
#include "Parser.h"

#include <ranges>
#include <stack>
#include <variant>

#include <iostream>


std::unique_ptr<std::map<std::string, Node>> Parser::parseTokens(const std::vector<Token>& tokens)
{
    state = State::OBJECT_PARSING;
    std::string key;

    auto nodes = std::make_unique<std::map<std::string, Node>>();
    currentNode = nodes.get();

    size_t counter = 1;
    for (const auto& token : tokens | std::views::drop(1)) {
        std::cout << counter << std::endl;
        counter++;
        if (token.type == TokenType::CURLY_CLOSE || token.type == TokenType::SQUARE_CLOSE) {
            std::cout << "curly close or square close" << std::endl;
            if (stackNodes.empty() == false) {
                currentNode = stackNodes.top();
                stackNodes.pop();
            }
            continue;
        }

        if (token.type == TokenType::COLON || token.type == TokenType::COMMA) {
            std::cout << "collon or comma" << std::endl;
            continue;
        }

        if (token.type == TokenType::KEY) {
            std::cout << "Parsing key" << std::endl;
            key = std::get<std::string>(token.data);
            continue;
        }

        if (token.type == TokenType::DATA_INT) {
            std::cout << "int" << std::endl;
            processData<int>(key, token);
        }
        else if (token.type == TokenType::DATA_DOUBLE) {
            std::cout << "double" << std::endl;
            processData<double>(key, token);
        }
        else if (token.type == TokenType::DATA_STR) {
            std::cout << "str" << std::endl;
            processData<std::string>(key, token);
        }
        else if (token.type == TokenType::DATA_BOOL) {
            std::cout << "bool" << std::endl;
            processData<bool>(key, token);
        }
        else if (token.type == TokenType::CURLY_OPEN) {
            std::cout << "curly open" << std::endl;
            pushObjectOnStack(key);
        }
        else if (token.type == TokenType::SQUARE_OPEN) {
            std::cout << "square open" << std::endl;
            pushArrayOnStack(key);
        }
        if (state == State::OBJECT_PARSING) {
            std::cout << "set parsing key to true" << std::endl;
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
        std::cout << "process data object" << std::endl;
        std::get<ObjectNode*>(currentNode)->emplace(std::make_pair(key, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(currentNode)->emplace_back(std::get<T>(token.data));
        std::cout << "process data array" << std::endl;
    }
}

