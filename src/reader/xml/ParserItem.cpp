
#include "ParserItem.h"


std::unique_ptr<std::vector<Item>> ParserTokens::parseTokens(std::unique_ptr<std::vector<TokenXML>> tokens)
{
    using enum ParsingState;
    items = std::make_unique<std::vector<Item>>();
    ParsingState state = ParsingState::STATE_NONE;

    for (auto token : *tokens) {
        switch (token.type)
        {
            case TokenTypeXML::ANGLE_OPEN:
                if (state == STATE_TAG_INITIAL || state == STATE_TAG_OPEN_PARSING) {
                    return nullptr;
                }
                state = STATE_TAG_INITIAL;
                break;
            case TokenTypeXML::ANGLE_CLOSE:
                if (state == STATE_TAG_OPEN_PARSING) {
                    state = STATE_TAG_OPEN_COMPLETED;
                }
                else if (state == STATE_TAG_CLOSE_NAMED) {
                    state = STATE_TAG_CLOSE_COMPLETED;
                }
                else {
                    return nullptr;
                }
                break;
            case TokenTypeXML::SLASH:
                if (state != STATE_TAG_INITIAL) {
                    return nullptr;
                }
                state = STATE_TAG_CLOSE_PARSING;
                break;
            case TokenTypeXML::DATA_STR:
                if (state == STATE_TAG_INITIAL) {
                    state = STATE_TAG_OPEN_PARSING;
                    items->emplace_back(ItemType::TAG_OPEN, std::make_optional(std::get<std::string>(token.data)), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    items->emplace_back(ItemType::TAG_CLOSE, std::make_optional(std::get<std::string>(token.data)), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_VALUE;
                    items->emplace_back(ItemType::VALUE, std::nullopt, std::vector<std::string>{ std::get<std::string>(token.data) });
                };
        }
    }
    return std::move(items);
}

