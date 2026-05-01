
#ifndef JSONPARSER_PREPARSER_H
#define JSONPARSER_PREPARSER_H

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "token.h"


class PreparserJSON
{
public:
    PreparserJSON() = default;

    std::unique_ptr<std::vector<Token>> parseJSON(const std::string& json);

private:
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
