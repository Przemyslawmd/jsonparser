
#include "parserTokensXML.h"

#include "error.h"
#include "log/ErrorStorage.h"


using enum ParsingState;

const std::map<ParsingState, ParsingState> angleCloseTransision = 
{
        { STATE_TAG_OPEN_PARSING,    STATE_TAG_OPEN_COMPLETED },
        { STATE_TAG_CLOSE_NAMED,     STATE_TAG_CLOSE_COMPLETED },
        { STATE_DECLARATION_CLOSING, STATE_DECLARATION_CLOSED },
};


const std::map<ParsingState, ParsingState> questionTransision = 
{
        { STATE_TAG_INITIAL,         STATE_DECLARATION_START },
        { STATE_DECLARATION_PARSING, STATE_DECLARATION_CLOSING },
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
                if (!questionTransision.contains(state)) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_QUESTION);
                    return nullptr;
                }
                state = questionTransision.at(state);
                break;
            case TokenTypeXML::DATA_STR:
            case TokenTypeXML::DATA_STR_QUOTA:
                if (state == STATE_TAG_INITIAL) {
                    state = STATE_TAG_OPEN_PARSING;
                    elems->emplace_back(ElemType::TAG_OPEN, std::get<std::string>(token.data), std::vector<TokenXML>{});
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    elems->emplace_back(ElemType::TAG_CLOSE, std::get<std::string>(token.data), std::vector<TokenXML>{});
                }
                else if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(ElemType::CONTENT, std::nullopt, std::vector<TokenXML>{{ token.type, token.data }});
                }
                else if (state == STATE_DECLARATION_PARSING || state == STATE_TAG_OPEN_PARSING) {
                    auto& tag = elems->back();
                    tag.attr.emplace_back(token.type, token.data);
                }
                else if (state == STATE_DECLARATION_START) {
                    elems->emplace_back(ElemType::DECLARATION, std::get<std::string>(token.data), std::vector<TokenXML>{});
                    state = STATE_DECLARATION_PARSING;
                }
                break;
            case TokenTypeXML::EQUAL:
                auto& tag = elems->back();
                tag.attr.emplace_back(token.type, token.data);
                break;
        }
    }
    return std::move(elems);
}

