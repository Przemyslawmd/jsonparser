
#include "Validator.h"

#include <format>
#include <map>
#include <set>
#include <stack>

#include "state.h"
#include "../log/TokenDescription.h"
#include "../log/ErrorStorage.h"


bool Validator::validate(const std::vector<Token>& tokens)
{
    if (tokens.front().type != TokenType::CURLY_OPEN) {
        ErrorStorage::putError(ErrorCode::VALIDATOR_IMPROPER_BEGIN);
        return false;
    }

    if (tokens.back().type != TokenType::CURLY_CLOSE) {
        ErrorStorage::putError(ErrorCode::VALIDATOR_IMPROPER_END);
        return false;
    }

    if (validateBrackets(tokens) == false) {
        return false;
    }
    return checkTokensSequence(tokens);
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
                ErrorStorage::putError(ErrorCode::VALIDATOR_BRACKET_CURLY);
                return false;
            }
        }
        else if (token.type == TokenType::SQUARE_CLOSE) {
            if (--squareCounter < 0) {
                ErrorStorage::putError(ErrorCode::VALIDATOR_BRACKET_SQUARE);
                return false;
            }
        }
    }

    if (curlyCounter != 0) {
        ErrorStorage::putError(ErrorCode::VALIDATOR_BRACKET_CURLY);
        return false;
    }
    if (squareCounter != 0) {
        ErrorStorage::putError(ErrorCode::VALIDATOR_BRACKET_SQUARE);
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
        { State::OBJECT_PARSING, { TokenType::COLON, TokenType::COMMA, TokenType::CURLY_CLOSE }},
        { State::ARRAY_PARSING,  { TokenType::COMMA, TokenType::SQUARE_CLOSE }}
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
        { State::ARRAY_PARSING,  { TokenType::CURLY_OPEN, TokenType::SQUARE_OPEN }}
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
        TokenType tokenType = it->type;
        TokenType nextTokenType = (it + 1)-> type;

        if (tokenType == TokenType::CURLY_OPEN) {
            states.push(State::OBJECT_PARSING);
            if (afterCurlyOpen.contains(nextTokenType) == false) {
                createError(ErrorCode::VALIDATOR_AFTER_CURLY_OPEN, tokenType, nextTokenType);
                return false;
            }
            continue;
        }
        if (tokenType == TokenType::SQUARE_OPEN) {
            states.push(State::ARRAY_PARSING);
            if (afterSquareOpen.contains(nextTokenType) == false) {
                createError(ErrorCode::VALIDATOR_AFTER_SQUARE_OPEN, tokenType, nextTokenType);
                return false;
            }
            continue;
        }
        if (tokenType == TokenType::CURLY_CLOSE) {
            states.pop();
            if (afterClose.contains(nextTokenType) == false) {
                createError(ErrorCode::VALIDATOR_AFTER_CURLY_CLOSE, tokenType, nextTokenType);
                return false;
            }
            continue;
        }
        if (tokenType == TokenType::SQUARE_CLOSE) {
            states.pop();
            if (afterClose.contains(nextTokenType) == false) {
                createError(ErrorCode::VALIDATOR_AFTER_CURLY_CLOSE, tokenType, nextTokenType);
                return false;
            }
            continue;
        }
        if (tokenType == TokenType::COMMA && afterComma.at(state).contains(nextTokenType) == false) {
            createError(ErrorCode::VALIDATOR_AFTER_COMMA, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == TokenType::COLON && (state == State::ARRAY_PARSING || afterColon.contains(nextTokenType) == false)) {
            createError(ErrorCode::VALIDATOR_AFTER_COLON, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == TokenType::DATA_STR && afterString.at(state).contains(nextTokenType) == false) {
            createError(ErrorCode::VALIDATOR_AFTER_STRING, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == TokenType::DATA_INT && afterData.contains(nextTokenType) == false) {
            createError(ErrorCode::VALIDATOR_AFTER_INT, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == TokenType::DATA_DOUBLE && afterData.contains(nextTokenType) == false) {
            createError(ErrorCode::VALIDATOR_AFTER_DOUBLE, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == TokenType::DATA_BOOL && afterData.contains(nextTokenType) == false) {
            createError(ErrorCode::VALIDATOR_AFTER_BOOL, tokenType, nextTokenType);
            return false;
        }
    }
    return true;
}


void Validator::createError(ErrorCode errorCode, TokenType first, TokenType second)
{
    ErrorStorage::putError(
        errorCode,
        std::format("Error details: {} after {}", TokenDesc.at(second), TokenDesc.at(first)));
}

