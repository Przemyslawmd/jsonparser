
#include "Validator.h"

#include <set>

#include <defines.h>


ParseError Validator::validate(const std::vector<Token>& tokens)
{
    ParseError error;

    if (tokens.at(0).type != TokenType::CURLY_OPEN) {
        return ParseError::FIRST_CHAR_NOT_CURLY_OPEN;
    }

    if (tokens.at(tokens.size() - 1).type != TokenType::CURLY_CLOSE) {
        return ParseError::LAST_CHAR_NOT_CURLY_CLOSE;
    }
    
    error = validateBrackets(tokens);
    if (error != ParseError::NOT_ERROR) {
        return error;
    }
    return checkRequirements(tokens);
}


ParseError Validator::validateBrackets(const std::vector<Token>& tokens)
{
    size_t curlyCounter = 0;
    size_t squareCounter = 0;

    for (const auto& token: tokens) {
        if (token.type == TokenType::CURLY_OPEN) {
            curlyCounter++;
        }
        else if (token.type == TokenType::CURLY_CLOSE) {
            curlyCounter--;
        }
        else if (token.type == TokenType::SQUARE_OPEN) {
            squareCounter++;
        }
        else if (token.type == TokenType::SQUARE_CLOSE) {
            squareCounter--;
        }

        if (curlyCounter < 0) {
            return ParseError::BRACKET_CURLY_ERROR;
        }
        if (squareCounter < 0) {
            return ParseError::BRACKET_SQUARE_ERROR;
        }
    }

    if (curlyCounter != 0) {
        return ParseError::BRACKET_CURLY_ERROR;
    }
    if (squareCounter != 0) {
        return ParseError::BRACKET_SQUARE_ERROR;
    }
    return ParseError::NOT_ERROR;
}


ParseError Validator::checkRequirements(const std::vector<Token>& tokens)
{
    const std::set<TokenType> allowedAfterString { TokenType::COLON, TokenType::COMMA, TokenType::CURLY_CLOSE };
    const std::set<TokenType> allowedAfterColon { TokenType::DATA_STR, TokenType::DATA_INT, TokenType::DATA_BOOL, TokenType::CURLY_OPEN };
    
    for (size_t index = 0; index < tokens.size(); index++) {
        if (tokens[index].type == TokenType::CURLY_OPEN && tokens[index + 1].type != TokenType::DATA_STR) {
            return ParseError::IMPROPER_TOKEN_AFTER_CURLY_OPEN;
        }
        if (tokens[index].type == TokenType::DATA_STR && allowedAfterString.count(tokens[index + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_STRING;
        }
        if (tokens[index].type == TokenType::COLON && allowedAfterColon.count(tokens[index + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_COLON;
        }
    }
    return ParseError::NOT_ERROR;
}


