
#include "ParserTokensXML.h"

#include "error.h"
#include "log/ErrorStorage.h"


const std::map<ParsingState, ParsingState> angleCloseTransision = 
{
        { ParsingState::STATE_TAG_OPEN_PARSING, ParsingState::STATE_TAG_OPEN_COMPLETED },
        { ParsingState::STATE_TAG_CLOSE_NAMED, ParsingState::STATE_TAG_CLOSE_COMPLETED },
        { ParsingState::STATE_DECLARATION_CLOSING, ParsingState::STATE_DECLARATION_CLOSED },
};


std::unique_ptr<std::vector<Elem>> ParserTokens::parseTokens(std::unique_ptr<std::vector<TokenXML>> tokens)
{
    if (tokens == nullptr || tokens->empty()) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_NO_TOKENS);
        return nullptr;
    }
    if (tokens->at(0).type != TokenTypeXML::ANGLE_OPEN) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_INVALID_BEGIN);
        return nullptr;
    }
    if (tokens->at(tokens->size() - 1).type != TokenTypeXML::ANGLE_CLOSE) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_INVALID_END);
        return nullptr;
    }

    using enum ParsingState;
    elems = std::make_unique<std::vector<Elem>>();
    ParsingState state = ParsingState::STATE_NONE;

    for (auto token : *tokens) {
        switch (token.type)
        {
            case TokenTypeXML::ANGLE_OPEN:
                if (state == STATE_TAG_INITIAL || state == STATE_TAG_OPEN_PARSING) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_OPEN_ANGLE);
                    return nullptr;
                }
                state = STATE_TAG_INITIAL;
                break;
            case TokenTypeXML::ANGLE_CLOSE:
                if (!angleCloseTransision.contains(state)) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_CLOSE_ANGLE);
                    return nullptr;
                }
                state = angleCloseTransision.at(state);
                break;
            case TokenTypeXML::SLASH:
                if (state != STATE_TAG_INITIAL && state != STATE_CONTENT) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_SLASH);
                    return nullptr;
                }
                state = STATE_TAG_CLOSE_PARSING;
                break;
            case TokenTypeXML::QUESTION:
                if (state == STATE_TAG_INITIAL) {
                    elems->emplace_back(ElemType::DECLARATION, std::nullopt, std::vector<std::string>{});
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
                    elems->emplace_back(ElemType::TAG_OPEN, std::get<std::string>(token.data), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    elems->emplace_back(ElemType::TAG_CLOSE, std::get<std::string>(token.data), std::vector<std::string>{});
                }
                else if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(ElemType::CONTENT, std::nullopt, std::vector<std::string>{ std::get<std::string>(token.data) });
                }
                else if (state == STATE_DECLARATION_PARSING) {
                    auto& tag = elems->back();
                    tag.data.push_back(std::get<std::string>(token.data));
                }
                break;
            case TokenTypeXML::EQUAL:
                auto& tag = elems->back();
                tag.data.push_back("=");
                break;
        }
    }
    return std::move(elems);
}

