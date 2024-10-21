
#ifndef JSONPARSER_PARSERKEY_H
#define JSONPARSER_PARSERKEY_H

#include <memory>
#include <stack>
#include <vector>

#include "state.h"
#include "Token.h"


static std::unique_ptr<std::vector<Token>> createKeyTokens(std::unique_ptr<std::vector<Token>> tokens)
{
    std::stack<State> states;

    for (auto it = tokens->begin(); it != tokens->end(); it++) {
        if (it->type == TokenType::CURLY_OPEN) {
            states.emplace(State::OBJECT_PARSING);
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            states.emplace(State::ARRAY_PARSING);
        }
        else if (it->type == TokenType::CURLY_CLOSE || it->type == TokenType::SQUARE_CLOSE) {
            states.pop();
        }
        else if (it->type == TokenType::DATA_STR &&
            states.top() == State::OBJECT_PARSING &&
            ((it - 1)->type == TokenType::CURLY_OPEN || (it - 1)->type == TokenType::COMMA)) {
            it->type = TokenType::KEY;
        }
    }
    return tokens;
}

#endif

