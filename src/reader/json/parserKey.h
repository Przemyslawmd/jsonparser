
#ifndef JX_READER_JSON_PARSERKEY_H
#define JX_READER_JSON_PARSERKEY_H

#include <memory>
#include <ranges>
#include <stack>
#include <vector>

#include "state.h"
#include "token.h"


namespace json
{
static std::unique_ptr<std::vector<Token>> createKeyTokens(std::unique_ptr<std::vector<Token>> tokens)
{
    std::stack<State> states;
    using enum TokenType;
    using enum State;

    for (const auto [idx, token] : std::views::enumerate(*tokens))
    {
        switch (token.type)
        {
            case CURLY_OPEN:
                states.emplace(OBJECT_PARSING);
                break;
            case SQUARE_OPEN:
                states.emplace(ARRAY_PARSING);
                break;
            case CURLY_CLOSE:
            case SQUARE_CLOSE:
                states.pop();
                break;
            case DATA_STR:
                auto prevType = tokens->at(idx - 1).type;
                if (states.top() == OBJECT_PARSING && (prevType == CURLY_OPEN || prevType == COMMA)) {
                    token.type = KEY;
                }
                break;
        }
    }
    return tokens;
}
}

#endif

