
#include "parserTokens.h"

#include <ranges>

#include "errorCode.h"
#include "log/ErrorStorage.h"


using namespace xml;

using enum ParsingState;
using enum TokenType;

const std::map<ParsingState, ParsingState> angleCloseTransision = 
{
        { STATE_TAG_OPEN_PARSING,    STATE_TAG_OPEN_COMPLETED },
        { STATE_TAG_CLOSE_NAMED,     STATE_TAG_CLOSE_COMPLETED },
};


std::unique_ptr<std::vector<ElemReader>> ParserTokens::parseTokens(std::unique_ptr<std::vector<Token>> tokens)
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

    elems = std::make_unique<std::vector<ElemReader>>();
    ParsingState state = ParsingState::STATE_NONE;

    std::optional<uint> declarationTokens = parseDeclaration(*tokens);
    if (!declarationTokens.has_value()) {
        ErrorStorage::putError(ErrorCode::XML_PARSER_TOKENS_DECLARATION);
        return nullptr;
    }

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
                    elems->emplace_back(ElemType::TAG_OPEN, std::get<std::string>(token.data));
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    elems->emplace_back(ElemType::TAG_CLOSE, std::get<std::string>(token.data));
                }
                else if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(ElemType::CONTENT, std::get<std::string>(token.data), token.data);
                }
                else if (state == STATE_TAG_OPEN_PARSING) {
                    auto& tag = elems->back();
                    tag.attr.emplace_back(token.type, token.data);
                }
                else if (state == STATE_CONTENT) {
                   auto& contentName = std::get<std::string>(elems->back().value);
                   elems->back().value = contentName + " " + std::get<std::string>(token.data);
                }
                break;
            case DATA_INT:
            case DATA_DOUBLE:
                if (state == STATE_TAG_CLOSE_COMPLETED || state == STATE_TAG_OPEN_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(ElemType::CONTENT, token.data);
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
constexpr std::string STA = "standalone";

std::optional<uint> ParserTokens::parseDeclaration(const std::vector<Token>& tokens)
{
    uint index = 1;
    if (tokens.at(index).type != QUESTION) {
        return 0;
    }
    if (tokens.at(index + 1).type != DATA_STR || std::get<std::string>(tokens.at(2).data) != XML) {
        return std::nullopt;
    }

    auto checkPair = [](const std::vector<Token>& tokens, uint index, const std::string& value)
    {
        return tokens.at(index).type == DATA_STR &&
               std::get<std::string>(tokens.at(index).data) == value &&
               tokens.at(index + 1).type == EQUAL &&
               tokens.at(index + 2).type == DATA_STR_QUOTA;
    };

    index = 3;
    if (!checkPair(tokens, index, VER)) {
        return std::nullopt;
    }
    const auto& verValue = std::get<std::string>(tokens.at(index + 2).data);
    if (verValue != "1.0" && verValue != "1.1") {
        return std::nullopt;
    }
    elems->emplace_back(ElemType::DECLARATION, XML, std::vector<Token> {{ DATA_STR, VER },
                                                                        { EQUAL, nullptr },
                                                                        { DATA_STR_QUOTA, verValue }});

    index = 6;
    if (tokens.at(index).type == QUESTION && tokens.at(index + 1).type == ANGLE_CLOSE) {
        return index;
    }

    if (!checkPair(tokens, index, ENC)) {
        return std::nullopt;
    }
    elems->back().attr.emplace_back(DATA_STR, ENC);
    elems->back().attr.emplace_back(EQUAL, nullptr);
    elems->back().attr.emplace_back(DATA_STR_QUOTA, std::get<std::string>(tokens.at(index + 2).data));

    index = 9;
    if (tokens.at(index).type == QUESTION && tokens.at(index + 1).type == ANGLE_CLOSE) {
        return index + 2;
    }

    if (!checkPair(tokens, index, STA)) {
        return std::nullopt;
    }
    const auto& staValue = std::get<std::string>(tokens.at(index + 2).data);
    if (staValue != "yes" && staValue != "no") {
        return std::nullopt;
    }
    elems->back().attr.emplace_back(DATA_STR, STA);
    elems->back().attr.emplace_back(EQUAL, nullptr);
    elems->back().attr.emplace_back(DATA_STR_QUOTA, staValue);

    index = 12;
    if (tokens.at(index).type == QUESTION && tokens.at(index + 1).type == ANGLE_CLOSE) {
        return index + 2;
    }
    return std::nullopt;
}

