
#ifndef READER_JSON_PREPARSER_H
#define READER_JSON_PREPARSER_H

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "token.h"

namespace json
{
class Preparser
{
public:
    Preparser() = default;

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
}

#endif
