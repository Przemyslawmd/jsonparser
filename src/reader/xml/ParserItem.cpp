
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
                else if (state == STATE_DECLARATION_CLOSING) {
                    state = STATE_DECLARATION_CLOSED;
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
            case TokenTypeXML::QUESTION:
                if (state == STATE_TAG_INITIAL) {
                    items->emplace_back(ItemType::DECLARATION, std::nullopt, std::vector<std::string>{});
                    state = STATE_DECLARATION_PARSING;
                }
                else if (state == STATE_DECLARATION_PARSING) {
                    state = STATE_DECLARATION_CLOSING;
                }
                break;
            case TokenTypeXML::DATA_STR:
            case TokenTypeXML::DATA_STR_QUOTA:
                if (state == STATE_TAG_INITIAL) {
                    state = STATE_TAG_OPEN_PARSING;
                    items->emplace_back(ItemType::TAG_OPEN, std::get<std::string>(token.data), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    items->emplace_back(ItemType::TAG_CLOSE, std::get<std::string>(token.data), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_VALUE;
                    items->emplace_back(ItemType::VALUE, std::nullopt, std::vector<std::string>{ std::get<std::string>(token.data) });
                }
                else if (state == STATE_DECLARATION_PARSING) {
                    auto& tag = items->back();
                    tag.data.push_back(std::get<std::string>(token.data));
                }
                break;
            case TokenTypeXML::EQUAL:
                auto& tag = items->back();
                tag.data.push_back("=");
                break;
        }
    }
    return std::move(items);
}

