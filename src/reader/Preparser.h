
#ifndef JSONPARSER_PREPARSER_H
#define JSONPARSER_PREPARSER_H

#include <map>
#include <memory>
#include <vector>

#include "state.h"
#include "Error.h"
#include "Token.h"


class Preparser
{
public:
    Preparser() = default;

    std::unique_ptr<std::vector<Token>> parseJSON(const std::string& json);

private:
    size_t parseNumber(const std::string& json, size_t index);
    size_t parseString(const std::string& json, size_t index);

    std::unique_ptr<std::vector<Token>> tokens;

    const std::map<char, TokenType> tokensMap {
        { '{', TokenType::CURLY_OPEN },
        { '}', TokenType::CURLY_CLOSE },
        { '[', TokenType::SQUARE_OPEN },
        { ']', TokenType::SQUARE_CLOSE },
        { ':', TokenType::COLON },
        { ',', TokenType::COMMA },
    };
};

#endif
