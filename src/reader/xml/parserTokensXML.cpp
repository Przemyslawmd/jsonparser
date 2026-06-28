
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

constexpr std::string XML = "xml";
constexpr std::string VER = "version";
constexpr std::string ENC = "encoding";

std::optional<uint> ParserTokens::parseDeclaration(const std::vector<TokenXML>& tokens)
{
    if (tokens.at(1).type != QUESTION) {
        return 0;
    }
    if (tokens.at(2).type != DATA_STR || std::get<std::string>(tokens.at(2).data) != XML) {
        return std::nullopt;
    }

    if (tokens.at(3).type != DATA_STR || 
        std::get<std::string>(tokens.at(3).data) != VER || 
        tokens.at(4).type != EQUAL || 
        tokens.at(5).type != DATA_STR_QUOTA) {
            return std::nullopt;
    }
    elems->emplace_back(ElemType::DECLARATION, XML, std::vector<TokenXML> {{ TokenTypeXML::DATA_STR, VER }, 
                                                                           { TokenTypeXML::EQUAL, nullptr }, 
                                                                           { TokenTypeXML::DATA_STR_QUOTA, std::get<std::string>(tokens.at(5).data) }});

    const auto checkAttrs = [](const std::vector<TokenXML>& tokens, uint questionIndex) 
    {
        return tokens.at(questionIndex).type == QUESTION && tokens.at(questionIndex + 1).type == ANGLE_CLOSE;
    };

    uint countOfAttrs;
    if (checkAttrs(tokens, 6)) {
        countOfAttrs = 1;
    }
    else if (checkAttrs(tokens, 9)) {
        countOfAttrs = 2;
    }
    else if (checkAttrs(tokens, 12)) {
        countOfAttrs = 3;
    }
    else {
        return std::nullopt;
    }

    if (countOfAttrs == 2) {
        if (tokens.at(6).type != DATA_STR || 
            std::get<std::string>(tokens.at(6).data) != ENC || 
            tokens.at(7).type != EQUAL || 
            tokens.at(8).type != DATA_STR_QUOTA) {
                return std::nullopt;
        }
        elems->back().attr.emplace_back(TokenTypeXML::DATA_STR, ENC);
        elems->back().attr.emplace_back(TokenTypeXML::EQUAL, nullptr);
        elems->back().attr.emplace_back(TokenTypeXML::DATA_STR_QUOTA, std::get<std::string>(tokens.at(8).data));
    }

    if (countOfAttrs == 2) {
        return 11;
    }
    if (countOfAttrs == 3) {
        return 14;
    }
    return std::nullopt;
}

