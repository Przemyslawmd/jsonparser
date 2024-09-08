
#include "Validator.h"

#include <format>
#include <map>
#include <set>
#include <stack>

#include <defines.h>
#include "../log/TokenDescription.h"


bool Validator::validate(const std::vector<Token>& tokens)
{
    if (tokens.at(0).type != TokenType::CURLY_OPEN) {
        error = std::make_unique<Error>(ErrorCode::VALIDATOR_IMPROPER_BEGINNING);
        return false;
    }

    if (tokens.at(tokens.size() - 1).type != TokenType::CURLY_CLOSE) {
        error = std::make_unique<Error>(ErrorCode::VALIDATOR_IMPROPER_END);
        return false;
    }

    if (validateBrackets(tokens) == false) {
        return false;
    }
    return checkTokensSequence(tokens);
}


std::unique_ptr<Error> Validator::getError()
{
    return std::move(error);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

bool Validator::validateBrackets(const std::vector<Token>& tokens)
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
                error = std::make_unique<Error>(ErrorCode::VALIDATOR_BRACKET_CURLY);
                return false;
            }
        }
        else if (token.type == TokenType::SQUARE_CLOSE) {
            if (--squareCounter < 0) {
                error = std::make_unique<Error>(ErrorCode::VALIDATOR_BRACKET_SQUARE);
                return false;
            }
        }
    }

    if (curlyCounter != 0) {
        error = std::make_unique<Error>(ErrorCode::VALIDATOR_BRACKET_CURLY);
        return false;
    }
    if (squareCounter != 0) {
        error = std::make_unique<Error>(ErrorCode::VALIDATOR_BRACKET_SQUARE);
        return false;
    }
    return true;
}


bool Validator::checkTokensSequence(const std::vector<Token>& tokens)
{
    std::stack<State> states;

    std::set<TokenType> tokenValue
    {
        TokenType::DATA_BOOL,
        TokenType::DATA_DOUBLE,
        TokenType::DATA_INT,
        TokenType::DATA_NULL,
        TokenType::DATA_STR
    };

    const std::set<TokenType> afterCurlyOpen 
    {
        TokenType::DATA_STR
    };

    std::set<TokenType> afterSquareOpen
    {
        TokenType::CURLY_OPEN,
        TokenType::SQUARE_OPEN,
    };
    afterSquareOpen.insert(tokenValue.begin(), tokenValue.end());

    const std::set<TokenType> afterClose
    {
        TokenType::COMMA,
        TokenType::CURLY_CLOSE,
        TokenType::SQUARE_CLOSE
    };

    const std::map<State, std::set<TokenType>> afterString
    {
        { State::OBJECT_PARSING, { TokenType::COLON,
                                   TokenType::COMMA,
                                   TokenType::CURLY_CLOSE }},
        { State::ARRAY_PARSING,  { TokenType::COMMA,
                                   TokenType::SQUARE_CLOSE }}
    };

    std::set<TokenType> afterColon
    {
        TokenType::CURLY_OPEN,
        TokenType::SQUARE_OPEN,
    };
    afterColon.insert(tokenValue.begin(), tokenValue.end());

    std::map<State, std::set<TokenType>> afterComma
    {
        { State::OBJECT_PARSING, { TokenType::DATA_STR }},
        { State::ARRAY_PARSING,  { TokenType::CURLY_OPEN,
                                   TokenType::SQUARE_OPEN }}
    };
    afterComma.at(State::ARRAY_PARSING).insert(tokenValue.begin(), tokenValue.end());

    const std::set<TokenType> afterData
    { 
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
                createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_OPEN, it->type, (it + 1)->type);
                return false;
            }
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            states.push(State::ARRAY_PARSING);
            if (afterSquareOpen.count((it + 1)->type) == 0) {
                createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_SQUARE_OPEN, it->type, (it + 1)->type);
                return false;
            }
        }
        else if (it->type == TokenType::CURLY_CLOSE) {
            states.pop();
            if (afterClose.count((it + 1)->type) == 0) {
                createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_CLOSE, it->type, (it + 1)->type);
                return false;
            }
        }
        else if (it->type == TokenType::SQUARE_CLOSE) {
            states.pop();
            if (afterClose.count((it + 1)->type) == 0) {
                createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_CLOSE, it->type, (it + 1)->type);
                return false;
            }
        }
        else if (it->type == TokenType::COMMA && afterComma.at(state).count((it + 1)->type) == 0) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_COMMA, it->type, (it + 1)->type);
            return false;
        }
        else if (it->type == TokenType::COLON && (state == State::ARRAY_PARSING || afterColon.count((it + 1)->type) == 0)) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_COLON, it->type, (it + 1)->type);
            return false;
        }
        else if (it->type == TokenType::DATA_STR && afterString.at(state).count((it + 1)->type) == 0) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_STRING, it->type, (it + 1)->type);
            return false;
        }
        else if (it->type == TokenType::DATA_INT && afterData.count((it + 1)->type) == 0) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_INT, it->type, (it + 1)->type);
            return false;
        }
        else if (it->type == TokenType::DATA_DOUBLE && afterData.count((it + 1)->type) == 0) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_DOUBLE, it->type, (it + 1)->type);
            return false;
        }
        else if (it->type == TokenType::DATA_BOOL && afterData.count((it + 1)->type) == 0) {
            createTypeAfterError(ErrorCode::VALIDATOR_TOKEN_AFTER_BOOL, it->type, (it + 1)->type);
            return false;
        }
    }
    return true;
}


void Validator::createTypeAfterError(ErrorCode errorCode, TokenType first, TokenType second)
{
    error = std::make_unique<Error>(
        errorCode,
        std::format("Error details: {} after {}", TokenDesc.at(second), TokenDesc.at(first)));
}

