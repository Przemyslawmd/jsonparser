
#include "Validator.h"

#include <map>
#include <set>
#include <stack>

#include <defines.h>


Result Validator::validate(const std::vector<Token>& tokens)
{
    Result error;

    if (tokens.at(0).type != TokenType::CURLY_OPEN) {
        return Result::VALIDATOR_IMPROPER_BEGINNING;
    }

    if (tokens.at(tokens.size() - 1).type != TokenType::CURLY_CLOSE) {
        return Result::VALIDATOR_IMPROPER_END;
    }

    error = validateBrackets(tokens);
    if (error != Result::OK) {
        return error;
    }
    return checkRequirements(tokens);
}


Result Validator::validateBrackets(const std::vector<Token>& tokens)
{
    int curlyCounter = 0;
    int squareCounter = 0;

    for (const auto& token : tokens) {
        if (token.type == TokenType::CURLY_OPEN) {
            curlyCounter++;
        }
        else if (token.type == TokenType::SQUARE_OPEN) {
            squareCounter++;
        }
        else if (token.type == TokenType::CURLY_CLOSE) {
            if (--curlyCounter < 0) {
                return Result::VALIDATOR_BRACKET_CURLY_ERROR;
            }
        }
        else if (token.type == TokenType::SQUARE_CLOSE) {
            if (--squareCounter < 0) {
                return Result::VALIDATOR_BRACKET_SQUARE_ERROR;
            }
        }
    }

    if (curlyCounter != 0) {
        return Result::VALIDATOR_BRACKET_CURLY_ERROR;
    }
    if (squareCounter != 0) {
        return Result::VALIDATOR_BRACKET_SQUARE_ERROR;
    }
    return Result::OK;
}


Result Validator::checkRequirements(const std::vector<Token>& tokens)
{
    std::stack<State> states;

    const std::set<TokenType> afterCurlyOpen {
        TokenType::DATA_STR
    };

    const std::set<TokenType> afterSquareOpen {
        TokenType::COLON,
        TokenType::CURLY_OPEN,
        TokenType::DATA_BOOL,
        TokenType::DATA_DOUBLE,
        TokenType::DATA_INT,
        TokenType::DATA_STR,
        TokenType::SQUARE_OPEN,
    };

    const std::set<TokenType> afterClose {
        TokenType::COMMA,
        TokenType::CURLY_CLOSE,
        TokenType::SQUARE_CLOSE
    };

    const std::map<State, std::set<TokenType>> afterString {
        { State::OBJECT_PARSING, { TokenType::COLON,
                                   TokenType::COMMA, 
                                   TokenType::CURLY_CLOSE }},
        { State::ARRAY_PARSING,  { TokenType::COMMA, 
                                   TokenType::SQUARE_CLOSE }}
    };

    const std::set<TokenType> afterColon {
        TokenType::DATA_STR,
        TokenType::DATA_INT,
        TokenType::DATA_DOUBLE,
        TokenType::DATA_BOOL,
        TokenType::CURLY_OPEN,
        TokenType::SQUARE_OPEN
    };

    const std::map<State, std::set<TokenType>> afterComma {
        { State::OBJECT_PARSING, { TokenType::DATA_STR }},
        { State::ARRAY_PARSING,  { TokenType::CURLY_OPEN, 
                                   TokenType::SQUARE_OPEN, 
                                   TokenType::DATA_STR, 
                                   TokenType::DATA_BOOL, 
                                   TokenType::DATA_INT, 
                                   TokenType::DATA_DOUBLE }}
    };

    const std::set<TokenType> afterData { 
        TokenType::COMMA, 
        TokenType::CURLY_CLOSE,
        TokenType::SQUARE_CLOSE
    };

    states.push(State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {

        State state = states.top();

        if (it->type == TokenType::CURLY_OPEN) {
            states.push(State::OBJECT_PARSING);
            if (afterCurlyOpen.count((it + 1)->type) == 0) {
                return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN;
            }
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            states.push(State::ARRAY_PARSING);
            if (afterSquareOpen.count((it + 1)->type) == 0) {
                return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_OPEN;
            }
        }
        else if (it->type == TokenType::CURLY_CLOSE) {
            states.pop();
            if (afterClose.count((it + 1)->type) == 0) {
                return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE;
            }
        }
        else if (it->type == TokenType::SQUARE_CLOSE) {
            states.pop();
            if (afterClose.count((it + 1)->type) == 0) {
                return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_SQUARE_CLOSE;
            }
        }
        else if (it->type == TokenType::COMMA && afterComma.at(state).count((it + 1)->type) == 0) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA;
        }
        else if (it->type == TokenType::COLON && (state == State::ARRAY_PARSING || afterColon.count((it + 1)->type) == 0)) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON;
        }
        else if (it->type == TokenType::DATA_STR && afterString.at(state).count((it + 1)->type) == 0) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING;
        }
        else if (it->type == TokenType::DATA_INT && afterData.count((it + 1)->type) == 0) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT;
        }
        else if (it->type == TokenType::DATA_DOUBLE && afterData.count((it + 1)->type) == 0) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE;
        }
        else if (it->type == TokenType::DATA_BOOL && afterData.count((it + 1)->type) == 0) {
            return Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL;
        }
    }
    return Result::OK;
}

