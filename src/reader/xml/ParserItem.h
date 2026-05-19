#ifndef JSONPARSER_PARSER_ITEM_XML_H
#define JSONPARSER_PARSER_ITEM_XML_H

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "token.h"
#include "item.h"


enum class ParsingState
{
    STATE_NONE,

    STATE_DECLARATION_PARSING,
    STATE_DECLARATION_CLOSING,
    STATE_DECLARATION_CLOSED,

    STATE_TAG_INITIAL,

    STATE_TAG_OPEN_PARSING,
    STATE_TAG_OPEN_COMPLETED,

    STATE_TAG_CLOSE_NAMED,
    STATE_TAG_CLOSE_PARSING,
    STATE_TAG_CLOSE_COMPLETED,

    STATE_VALUE
};


class ParserTokens
{
public:
    ParserTokens() = default;

    std::unique_ptr<std::vector<Item>> parseTokens(std::unique_ptr<std::vector<TokenXML>> tokens);

private:
    std::unique_ptr<std::vector<Item>> items;
};

#endif

