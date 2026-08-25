
#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "token.h"
#include "../../headers/elem.h"


namespace xml
{
enum class ParsingState
{
    STATE_NONE,

    STATE_ANGLE_OPEN,
    STATE_TAG_OPEN_NAMED,

    STATE_TAG_CLOSE_NAMED,
    STATE_TAG_CLOSE_PARSING,
    STATE_TAG_COMPLETED,

    STATE_ATTR_KEY,
    STATE_ATTR_EQUAL,
    STATE_ATTR_VALUE,

    STATE_CONTENT
};


class ParserTokens
{
public:
    ParserTokens() = default;

    std::unique_ptr<std::vector<Elem>> parseTokens(std::unique_ptr<std::vector<Token>> tokens);

private:
    std::unique_ptr<std::vector<Elem>> elems;

    std::optional<unsigned int> parseDeclaration(const std::vector<Token>& tokens);
};
}

