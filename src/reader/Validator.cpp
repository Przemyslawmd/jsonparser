
#include "Validator.h"

#include <format>
#include <map>
#include <set>
#include <stack>

#include "state.h"
#include "log/TokenDescription.h"
#include "log/ErrorStorage.h"


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

    ErrorCode error = validateBrackets(tokens);
    if (error != ErrorCode::NO_ERROR) {
        ErrorStorage::putError(error);
        return false;
    }
    return checkTokensSequence(tokens);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

ErrorCode Validator::validateBrackets(const std::vector<Token>& tokens)
{
    using enum ErrorCode;
    using enum TokenType;
    int curlyCounter = 0;
    int squareCounter = 0;

    for (const auto& token : tokens) {
        if (token.type == CURLY_OPEN) {
            curlyCounter++;
        }
        else if (token.type == SQUARE_OPEN) {
            squareCounter++;
        }
        else if (token.type == CURLY_CLOSE) {
            if (--curlyCounter < 0) {
                return VALIDATOR_BRACKET_CURLY;
            }
        }
        else if (token.type == SQUARE_CLOSE) {
            if (--squareCounter < 0) {
                return VALIDATOR_BRACKET_SQUARE;
            }
        }
    }

    if (curlyCounter != 0) {
        return VALIDATOR_BRACKET_CURLY;
    }
    if (squareCounter != 0) {
        return VALIDATOR_BRACKET_SQUARE;
    }
    return NO_ERROR;
}


bool Validator::checkTokensSequence(const std::vector<Token>& tokens)
{
    using enum ErrorCode;
    using enum TokenType;
    using enum State;

    std::stack<State> states;

    std::set<TokenType> tokenValue
    {
        DATA_BOOL,
        DATA_DOUBLE,
        DATA_INT,
        DATA_NULL,
        DATA_STR
    };

    const std::set<TokenType> afterCurlyOpen 
    {
        DATA_STR
    };

    std::set<TokenType> afterSquareOpen
    {
        CURLY_OPEN,
        SQUARE_OPEN,
    };
    afterSquareOpen.insert(tokenValue.begin(), tokenValue.end());

    const std::set<TokenType> afterClose
    {
        COMMA,
        CURLY_CLOSE,
        SQUARE_CLOSE
    };

    const std::map<State, std::set<TokenType>> afterString
    {
        { OBJECT_PARSING, { COLON, COMMA, CURLY_CLOSE }},
        { ARRAY_PARSING,  { COMMA, SQUARE_CLOSE }}
    };

    std::set<TokenType> afterColon
    {
        CURLY_OPEN,
        SQUARE_OPEN,
    };
    afterColon.insert(tokenValue.begin(), tokenValue.end());

    std::map<State, std::set<TokenType>> afterComma
    {
        { OBJECT_PARSING, { DATA_STR }},
        { ARRAY_PARSING,  { CURLY_OPEN, SQUARE_OPEN }}
    };
    afterComma.at(State::ARRAY_PARSING).insert(tokenValue.begin(), tokenValue.end());

    const std::set<TokenType> afterData
    { 
        COMMA,
        CURLY_CLOSE,
        SQUARE_CLOSE
    };

    states.push(State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {

        State state = states.top();
        TokenType tokenType = it->type;
        TokenType nextTokenType = (it + 1)-> type;

        if (tokenType == CURLY_OPEN) {
            states.push(State::OBJECT_PARSING);
            if (afterCurlyOpen.contains(nextTokenType)) {
                continue;
            }
            createError(VALIDATOR_AFTER_CURLY_OPEN, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == SQUARE_OPEN) {
            states.push(ARRAY_PARSING);
            if (afterSquareOpen.contains(nextTokenType)) {
                continue;
            }
            createError(VALIDATOR_AFTER_SQUARE_OPEN, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == CURLY_CLOSE) {
            states.pop();
            if (afterClose.contains(nextTokenType)) {
                continue;
            }
            createError(VALIDATOR_AFTER_CURLY_CLOSE, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == SQUARE_CLOSE) {
            states.pop();
            if (afterClose.contains(nextTokenType)) {
                continue;
            }
            createError(VALIDATOR_AFTER_CURLY_CLOSE, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == COMMA && afterComma.at(state).contains(nextTokenType) == false) {
            createError(VALIDATOR_AFTER_COMMA, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == COLON && (state == ARRAY_PARSING || afterColon.contains(nextTokenType) == false)) {
            createError(VALIDATOR_AFTER_COLON, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == DATA_STR && afterString.at(state).contains(nextTokenType) == false) {
            createError(VALIDATOR_AFTER_STRING, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == DATA_INT && afterData.contains(nextTokenType) == false) {
            createError(VALIDATOR_AFTER_INT, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == DATA_DOUBLE && afterData.contains(nextTokenType) == false) {
            createError(VALIDATOR_AFTER_DOUBLE, tokenType, nextTokenType);
            return false;
        }
        if (tokenType == DATA_BOOL && afterData.contains(nextTokenType) == false) {
            createError(VALIDATOR_AFTER_BOOL, tokenType, nextTokenType);
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

