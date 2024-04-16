
#include "ParserKey.h"

#include <stack>


std::unique_ptr<std::vector<Token>> ParserKey::createKeyTokens(std::unique_ptr<std::vector<Token>> tokens)
{
    std::stack<State> states;

    for (size_t index = 0; index < tokens->size(); index++) {
        if (tokens->at(index).type == TokenType::CURLY_OPEN) {
            states.emplace(State::OBJECT_PARSING);
        }
        else if (tokens->at(index).type == TokenType::SQUARE_OPEN) {
            states.emplace(State::ARRAY_PARSING);
        }
        else if (tokens->at(index).type == TokenType::CURLY_CLOSE || tokens->at(index).type == TokenType::SQUARE_CLOSE) {
            states.pop();
        }
        else if ((tokens->at(index).type == TokenType::DATA_STR) &&
            states.top() == State::OBJECT_PARSING &&
            (tokens->at(index - 1).type == TokenType::CURLY_OPEN || tokens->at(index - 1).type == TokenType::COMMA)) {
            tokens->at(index).type = TokenType::KEY;
        }
    }
    return tokens;
}
