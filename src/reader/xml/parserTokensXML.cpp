
#include "parserTokensXML.h"

#include <ranges>

#include "error.h"
#include "log/ErrorStorage.h"


using enum ParsingState;
using enum TokenTypeXML;

const std::map<ParsingState, ParsingState> angleCloseTransision = 
{
        { STATE_TAG_OPEN_PARSING,    STATE_TAG_OPEN_COMPLETED },
        { STATE_TAG_CLOSE_NAMED,     STATE_TAG_CLOSE_COMPLETED },
};


std::unique_ptr<std::vector<Elem>> ParserTokens::parseTokens(std::unique_ptr<std::vector<TokenXML>> tokens)
{
    if (tokens == nullptr || tokens->empty()) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_NO_TOKENS);
        return nullptr;
    }
    if (tokens->at(0).type != ANGLE_OPEN) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_INVALID_BEGIN);
        return nullptr;
    }
    if (tokens->at(tokens->size() - 1).type != ANGLE_CLOSE) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_INVALID_END);
        return nullptr;
    }

    elems = std::make_unique<std::vector<Elem>>();
    ParsingState state = ParsingState::STATE_NONE;
    std::optional<uint> declarationTokens = parseDeclaration(*tokens);

    for (auto token : *tokens | std::views::drop(declarationTokens.value())) {
        switch (token.type)
        {
            case ANGLE_OPEN:
                if (state == STATE_TAG_INITIAL || state == STATE_TAG_OPEN_PARSING) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_OPEN_ANGLE);
                    return nullptr;
                }
                state = STATE_TAG_INITIAL;
                break;
            case ANGLE_CLOSE:
                if (!angleCloseTransision.contains(state)) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_CLOSE_ANGLE);
                    return nullptr;
                }
                state = angleCloseTransision.at(state);
                break;
            case SLASH:
                if (state != STATE_TAG_INITIAL && state != STATE_CONTENT) {
                    ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_SLASH);
                    return nullptr;
                }
                state = STATE_TAG_CLOSE_PARSING;
                break;
            case QUESTION:
                ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_QUESTION);
                return nullptr;
            case DATA_STR:
            case DATA_STR_QUOTA:
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
                    elems->emplace_back(ElemType::CONTENT, std::get<std::string>(token.data), std::vector<TokenXML>{{ token.type, token.data }});
                }
                else if (state == STATE_TAG_OPEN_PARSING) {
                    auto& tag = elems->back();
                    tag.attr.emplace_back(token.type, token.data);
                }
                break;
            case EQUAL:
                auto& tag = elems->back();
                tag.attr.emplace_back(token.type, token.data);
                break;
        }
    }
    return std::move(elems);
}


std::optional<uint> ParserTokens::parseDeclaration(const std::vector<TokenXML>& tokens)
{
    if (tokens.at(1).type != QUESTION) {
        return 0;
    }
    if (tokens.at(2).type != DATA_STR) {
        return std::nullopt;
    }
    if (std::get<std::string>(tokens.at(2).data) != "xml") {
        return std::nullopt;
    }

    if (tokens.at(3).type != DATA_STR) {
        return std::nullopt;
    }
    if (std::get<std::string>(tokens.at(3).data) != "version") {
        return std::nullopt;
    }
    if (tokens.at(4).type != EQUAL) {
        return std::nullopt;
    }
    if (tokens.at(5).type != DATA_STR_QUOTA) {
        return std::nullopt;
    }
    elems->emplace_back(ElemType::DECLARATION, "xml", std::vector<TokenXML>
        {{ TokenTypeXML::DATA_STR, "version" }, { TokenTypeXML::EQUAL, nullptr }, { TokenTypeXML::DATA_STR_QUOTA, std::get<std::string>(tokens.at(5).data) }});

    uint countOfAttrs;
    if (tokens.at(7).type == QUESTION && tokens.at(8).type == ANGLE_CLOSE) {
        countOfAttrs = 1;
    }
    else if (tokens.at(9).type == QUESTION && tokens.at(10).type == ANGLE_CLOSE) {
        countOfAttrs = 2;
    }
    else if (tokens.at(12).type == QUESTION && tokens.at(13).type == ANGLE_CLOSE) {
        countOfAttrs = 3;
    }
    else {
        return std::nullopt;
    }

    if (countOfAttrs == 2) {
        if (tokens.at(6).type != DATA_STR) {
        return std::nullopt;
        }
        if (std::get<std::string>(tokens.at(6).data) != "encoding") {
            return std::nullopt;
        }
        if (tokens.at(7).type != EQUAL) {
            return std::nullopt;
        }
        if (tokens.at(8).type != DATA_STR_QUOTA) {
            return std::nullopt;
        }
        elems->back().attr.emplace_back(TokenTypeXML::DATA_STR, "encoding");
        elems->back().attr.emplace_back(TokenTypeXML::EQUAL, nullptr);
        elems->back().attr.emplace_back(TokenTypeXML::DATA_STR_QUOTA, std::get<std::string>(tokens.at(8).data));
    }

    if (countOfAttrs == 2) {
        return 10 + 1;
    }
    if (countOfAttrs == 3) {
        return 13 + 1;
    }
    return std::nullopt;
}

