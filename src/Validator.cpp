
#include "Validator.h"

#include <defines.h>


// Rules 
// after curly_open must be string


ParseError Validator::validate(std::vector<Token>* tokens)
{
    ParseError error;

    if (tokens->at(0).type != TokenType::CURLY_OPEN) {
        return ParseError::FIRST_CHAR_NOT_CURLY_OPEN;
    }

     if (tokens->at(tokens->size() - 1).type != TokenType::CURLY_CLOSE) {
        return ParseError::LAST_CHAR_NOT_CURLY_CLOSE;
    }
    
    error = validateBrackets(tokens);
    if (error != ParseError::NOT_ERROR) {
        return error;
    }
    return ParseError::NOT_ERROR;
}


ParseError Validator::validateBrackets(std::vector<Token>* tokens)
{
    size_t curlyCounter = 0;
    size_t squareCounter = 0;

    for (const auto& token: *tokens) {
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
            return ParseError::CURLY_BRACKER_ERROR;
        }
        if (squareCounter < 0) {
            return ParseError::SQUARE_BRACKET_ERROR;
        }
    }

    if (curlyCounter != 0) {
        return ParseError::CURLY_BRACKER_ERROR;
    }
    if (squareCounter != 0) {
        return ParseError::SQUARE_BRACKET_ERROR;
    }
    return ParseError::NOT_ERROR;
}

