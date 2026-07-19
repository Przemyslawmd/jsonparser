
#ifndef JX_READER_XML_PARSER_TOKENS_H
#define JX_READER_XML_PARSER_TOKENS_H

#include <map>
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "token.h"
#include "elem.h"


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

    STATE_EQUAL,

    STATE_CONTENT
};


class ParserTokens
{
public:
    ParserTokens() = default;

    std::unique_ptr<std::vector<ElemReader>> parseTokens(std::unique_ptr<std::vector<Token>> tokens);

private:
    std::unique_ptr<std::vector<ElemReader>> elems;

    std::optional<uint> parseDeclaration(const std::vector<Token>& tokens);
};
}

#endif

