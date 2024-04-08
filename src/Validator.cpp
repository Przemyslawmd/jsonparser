
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

    for (const auto& token : tokens) {
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
    const std::set<TokenType> afterString { 
        TokenType::COLON, 
        TokenType::COMMA, 
        TokenType::CURLY_CLOSE,
        TokenType::SQUARE_CLOSE };
    const std::set<TokenType> afterData { 
        TokenType::COMMA, 
        TokenType::CURLY_CLOSE };
    const std::set<TokenType> afterColon { 
        TokenType::DATA_STR, 
        TokenType::DATA_INT, 
        TokenType::DATA_DOUBLE, 
        TokenType::DATA_BOOL, 
        TokenType::CURLY_OPEN,
        TokenType::SQUARE_OPEN };
    const std::set<TokenType> afterCurlyClose { 
        TokenType::COMMA, 
        TokenType::CURLY_CLOSE };

    for (size_t i = 1; i < tokens.size() - 1; i++) {
        if (tokens[i].type == TokenType::CURLY_OPEN && tokens[i + 1].type != TokenType::DATA_STR) {
            return ParseError::IMPROPER_TOKEN_AFTER_CURLY_OPEN;
        }
        if (tokens[i].type == TokenType::CURLY_CLOSE && afterCurlyClose.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_CURLY_CLOSE;
        }
        if (tokens[i].type == TokenType::COMMA && tokens[i + 1].type != TokenType::DATA_STR) {
            return ParseError::IMPROPER_TOKEN_AFTER_COMMA;
        }
        if (tokens[i].type == TokenType::COLON && afterColon.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_COLON;
        }
        if (tokens[i].type == TokenType::DATA_STR && afterString.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_STRING;
        }
        if (tokens[i].type == TokenType::DATA_INT && afterData.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_DATA_INT;
        }
        if (tokens[i].type == TokenType::DATA_DOUBLE && afterData.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_DATA_DOUBLE;
        }
        if (tokens[i].type == TokenType::DATA_BOOL && afterData.count(tokens[i + 1].type) == 0) {
            return ParseError::IMPROPER_TOKEN_AFTER_DATA_BOOL;
        }
    }
    return ParseError::NOT_ERROR;
}

