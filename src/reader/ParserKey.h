
#ifndef JSONPARSER_PARSERKEY_H
#define JSONPARSER_PARSERKEY_H

#include <memory>
#include <stack>
#include <vector>

#include "state.h"
#include "token.h"


static std::unique_ptr<std::vector<Token>> createKeyTokens(std::unique_ptr<std::vector<Token>> tokens)
{
    std::stack<State> states;
    using enum TokenType;
    using enum State;

    for (auto it = tokens->begin(); it != tokens->end(); it++) {
        if (it->type == CURLY_OPEN) {
            states.emplace(OBJECT_PARSING);
        }
        else if (it->type == SQUARE_OPEN) {
            states.emplace(ARRAY_PARSING);
        }
        else if (it->type == CURLY_CLOSE || it->type == SQUARE_CLOSE) {
            states.pop();
        }
        else if (it->type == DATA_STR &&
                 states.top() == OBJECT_PARSING &&
                 ((it - 1)->type == CURLY_OPEN || (it - 1)->type == COMMA)) {
            it->type = KEY;
        }
    }
    return tokens;
}

#endif

