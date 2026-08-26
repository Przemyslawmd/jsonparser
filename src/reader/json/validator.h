
#pragma once

#include <format>
#include <map>
#include <ranges>
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

    std::set<TokenType> tokenData
    {
        DATA_BOOL,
        DATA_DOUBLE,
        DATA_INT,
        DATA_NULL,
        DATA_STR
    };

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

    std::set<TokenType> afterColonOrSquareOpen
    {
        CURLY_OPEN,
        SQUARE_OPEN,
    };
    afterColonOrSquareOpen.insert(tokenData.begin(), tokenData.end());

    std::map<State, std::set<TokenType>> afterComma
    {
        { OBJECT_PARSING, { DATA_STR }},
        { ARRAY_PARSING,  { CURLY_OPEN, SQUARE_OPEN }}
    };
    afterComma.at(ARRAY_PARSING).insert(tokenData.begin(), tokenData.end());

    const std::set<TokenType> afterData
    {
        COMMA,
        CURLY_CLOSE,
        SQUARE_CLOSE
    };

    states.push(OBJECT_PARSING);

    for (const auto& [idx, token] : std::views::enumerate(tokens) | std::views::take(tokens.size() - 1)) {

        State state = states.top();
        TokenType type = token.type;
        TokenType nextType = tokens[idx + 1].type;

        switch (type)
        {
            case CURLY_OPEN:
                states.push(OBJECT_PARSING);
                if (nextType == DATA_STR) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_CURLY_OPEN, type, nextType);
                return false;
            case SQUARE_OPEN:
                states.push(ARRAY_PARSING);
                if (afterColonOrSquareOpen.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_SQUARE_OPEN, type, nextType);
                return false;
            case CURLY_CLOSE:
                states.pop();
                if (afterClose.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_CURLY_CLOSE, type, nextType);
                return false;
            case SQUARE_CLOSE:
                states.pop();
                if (afterClose.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_SQUARE_CLOSE, type, nextType);
                return false;
            case COMMA:
                if (afterComma.at(state).contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_COMMA, type, nextType);
                return false;
            case COLON:
                if (state != ARRAY_PARSING && afterColonOrSquareOpen.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_COLON, type, nextType);
                return false;
            case DATA_STR:
                if (afterString.at(state).contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_STRING, type, nextType);
                return false;
            case DATA_INT:
                if (afterData.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_INT, type, nextType);
                return false;
            case DATA_DOUBLE:
                if (afterData.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_DOUBLE, type, nextType);
                return false;
            case DATA_BOOL:
                if (afterData.contains(nextType)) {
                    continue;
                }
                createError(JSON_VALIDATOR_AFTER_BOOL, type, nextType);
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
                    return JSON_VALIDATOR_BRACKET_CURLY;
                }
                continue;
            case SQUARE_CLOSE:
                if (--squareCounter < 0) {
                    return JSON_VALIDATOR_BRACKET_SQUARE;
                }
        }
    }

    if (curlyCounter != 0) {
        return JSON_VALIDATOR_BRACKET_CURLY;
    }
    if (squareCounter != 0) {
        return JSON_VALIDATOR_BRACKET_SQUARE;
    }
    return NO_ERROR;
}


static bool validateTokens(const std::vector<Token>& tokens)
{
    using enum ErrorCode;
    using enum TokenType;

    if (tokens.front().type != CURLY_OPEN) {
        ErrorStorage::putError(JSON_VALIDATOR_IMPROPER_BEGIN);
        return false;
    }
    if (tokens.back().type != CURLY_CLOSE) {
        ErrorStorage::putError(JSON_VALIDATOR_IMPROPER_END);
        return false;
    }

    if  (const ErrorCode error = validateBrackets(tokens); error != NO_ERROR) {
        ErrorStorage::putError(error);
        return false;
    }
    return checkTokensSequence(tokens);
}
}

