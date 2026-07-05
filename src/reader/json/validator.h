
#ifndef JX_READER_JSON_VALIDATOR_H
#define JX_READER_JSON_VALIDATOR_H

#include <format>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "state.h"
#include "token.h"
#include "errorCode.h"
#include "log/TokenDescription.h"
#include "log/ErrorStorage.h"


namespace json
{

static void createError(ErrorCode code, TokenType first, TokenType second)
{
   ErrorStorage::putError(
        code,
        std::format("Error details: {} after {}", TokenDesc.at(second), TokenDesc.at(first)));
}


static bool checkTokensSequence(const std::vector<Token>& tokens)
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

        switch (tokenType)
        {
            case CURLY_OPEN:
                states.push(State::OBJECT_PARSING);
                if (afterCurlyOpen.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_CURLY_OPEN, tokenType, nextTokenType);
                return false;
            case SQUARE_OPEN:
                states.push(ARRAY_PARSING);
                if (afterSquareOpen.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_SQUARE_OPEN, tokenType, nextTokenType);
                return false;
            case CURLY_CLOSE:
                states.pop();
                if (afterClose.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_CURLY_CLOSE, tokenType, nextTokenType);
                return false;
            case SQUARE_CLOSE:
                states.pop();
                if (afterClose.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_SQUARE_CLOSE, tokenType, nextTokenType);
                return false;
            case COMMA:
                if (afterComma.at(state).contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_COMMA, tokenType, nextTokenType);
                return false;
            case COLON:
                if (state != ARRAY_PARSING && afterColon.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_COLON, tokenType, nextTokenType);
                return false;
            case DATA_STR:
                if (afterString.at(state).contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_STRING, tokenType, nextTokenType);
                return false;
            case DATA_INT:
                if (afterData.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_INT, tokenType, nextTokenType);
                return false;
            case DATA_DOUBLE:
                if (afterData.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_DOUBLE, tokenType, nextTokenType);
                return false;
            case DATA_BOOL:
                if (afterData.contains(nextTokenType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_BOOL, tokenType, nextTokenType);
                return false;
        }
    }
    return true;
}


static ErrorCode validateBrackets(const std::vector<Token>& tokens)
{
    using enum ErrorCode;
    using enum TokenType;

    int curlyCounter = 0;
    int squareCounter = 0;

    for (const auto& token : tokens) {
        switch (token.type)
        {
            case CURLY_OPEN:
                curlyCounter++;
                continue;
            case SQUARE_OPEN:
                squareCounter++;
                continue;
            case CURLY_CLOSE:
                if (--curlyCounter < 0) {
                    return VALIDATOR_BRACKET_CURLY;
                }
                continue;
            case SQUARE_CLOSE:
                if (--squareCounter < 0) {
                    return VALIDATOR_BRACKET_SQUARE;
                }
                continue;
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


static bool validateTokens(const std::vector<Token>& tokens)
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
}

#endif

