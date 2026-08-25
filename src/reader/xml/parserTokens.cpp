
#include "parserTokens.h"

#include <ranges>

#include "definesXML.h"
#include "errorCode.h"
#include "log/ErrorStorage.h"


using namespace xml;

using enum ElemType;
using enum ErrorCode;
using enum ParsingState;
using enum TokenType;


const std::map<ParsingState, ParsingState> angleCloseTransision = 
{
    { STATE_TAG_OPEN_NAMED,  STATE_TAG_COMPLETED },
    { STATE_TAG_CLOSE_NAMED, STATE_TAG_COMPLETED },
    { STATE_ATTR_VALUE,      STATE_TAG_COMPLETED },
};


std::unique_ptr<std::vector<ElemReader>> ParserTokens::parseTokens(std::unique_ptr<std::vector<Token>> tokens)
{
    if (!tokens || tokens->empty()) {
        ErrorStorage::putError(XML_PARSER_TOKENS_NO_TOKENS);
        return nullptr;
    }
    if (tokens->front().type != ANGLE_OPEN) {
        ErrorStorage::putError(XML_PARSER_TOKENS_INVALID_BEGIN);
        return nullptr;
    }
    if (tokens->back().type != ANGLE_CLOSE) {
        ErrorStorage::putError(XML_PARSER_TOKENS_INVALID_END);
        return nullptr;
    }

    elems = std::make_unique<std::vector<ElemReader>>();
    ParsingState state = STATE_NONE;

    auto declarationTokens = parseDeclaration(*tokens);
    if (!declarationTokens.has_value()) {
        ErrorStorage::putError(XML_PARSER_TOKENS_DECLARATION);
        return nullptr;
    }

    std::optional<std::string> attrKey = std::nullopt;

    for (auto token : *tokens | std::views::drop(declarationTokens.value())) {
        switch (token.type)
        {
            case ANGLE_OPEN:
                if (state == STATE_ANGLE_OPEN || state == STATE_TAG_OPEN_NAMED) {
                    ErrorStorage::putError(XML_PARSER_TOKENS_OPEN_ANGLE);
                    return nullptr;
                }
                state = STATE_ANGLE_OPEN;
                break;
            case ANGLE_CLOSE:
                if (!angleCloseTransision.contains(state)) {
                    ErrorStorage::putError(XML_PARSER_TOKENS_CLOSE_ANGLE);
                    return nullptr;
                }
                state = angleCloseTransision.at(state);
                break;
            case SLASH:
                if (state != STATE_ANGLE_OPEN && state != STATE_CONTENT) {
                    ErrorStorage::putError(XML_PARSER_TOKENS_SLASH);
                    return nullptr;
                }
                state = STATE_TAG_CLOSE_PARSING;
                break;
            case QUESTION:
                ErrorStorage::putError(XML_PARSER_TOKENS_QUESTION);
                return nullptr;
            case DATA_STR:
                if (state == STATE_ANGLE_OPEN) {
                    state = STATE_TAG_OPEN_NAMED;
                    elems->emplace_back(TAG_OPEN, std::get<std::string>(token.data));
                }
                else if (state == STATE_TAG_CLOSE_PARSING) {
                    state = STATE_TAG_CLOSE_NAMED;
                    elems->emplace_back(TAG_CLOSE, std::get<std::string>(token.data));
                }
                else if (state == STATE_TAG_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(CONTENT, std::get<std::string>(token.data), token.data);
                }
                else if (state == STATE_ATTR_VALUE || state == STATE_TAG_OPEN_NAMED) {
                    attrKey = std::get<std::string>(token.data);
                    state = STATE_ATTR_KEY;
                }
                else if (state == STATE_CONTENT) {
                   auto& contentName = std::get<std::string>(elems->back().value);
                   elems->back().value = contentName + " " + std::get<std::string>(token.data);
                }
                else {
                    ErrorStorage::putError(XML_PARSER_TOKENS_DATA_STR);
                    return nullptr;
                }
                break;
            case DATA_STR_QUOTA:
                if (state == STATE_ATTR_EQUAL) {
                    auto& tag = elems->back();
                    tag.attr.emplace(attrKey.value(), std::get<std::string>(token.data));
                    state = STATE_ATTR_VALUE;
                    break;
                    }
                ErrorStorage::putError(XML_PARSER_TOKENS_DATA_STR_QUOTA);
                return nullptr;
            case DATA_INT:
            case DATA_DOUBLE:
                if (state == STATE_TAG_COMPLETED) {
                    state = STATE_CONTENT;
                    elems->emplace_back(CONTENT, token.data);
                }
                break;
            case EQUAL:
                if (state != STATE_ATTR_KEY) {
                    ErrorStorage::putError(XML_PARSER_TOKENS_EQUAL);
                    return nullptr;
                }
                state = STATE_ATTR_EQUAL;
                break;
        }
    }
    return std::move(elems);
}


std::optional<unsigned int> ParserTokens::parseDeclaration(const std::vector<Token>& tokens)
{
    auto checkPair = [](const std::vector<Token>& tokens, unsigned int index, const std::string& value)
    {
        return tokens.at(index).type == DATA_STR &&
               std::get<std::string>(tokens.at(index).data) == value &&
               tokens.at(index + 1).type == EQUAL &&
               tokens.at(index + 2).type == DATA_STR_QUOTA;
    };

    auto checkClosing = [](const std::vector<Token>& tokens, unsigned int index)
    {
        return tokens.at(index).type == QUESTION && tokens.at(index + 1).type == ANGLE_CLOSE;
    };

    unsigned int index = 1;
    if (tokens.at(index).type != QUESTION) {
        return 0;
    }
    if (tokens.at(index + 1).type != DATA_STR || std::get<std::string>(tokens.at(index + 1).data) != XML) {
        return std::nullopt;
    }

    index = 3;
    if (!checkPair(tokens, index, VER)) {
        return std::nullopt;
    }
    const auto& verValue = std::get<std::string>(tokens.at(index + 2).data);
    if (verValue != "1.0" && verValue != "1.1") {
        return std::nullopt;
    }
    auto& elem = elems->emplace_back(DECLARATION, XML);
    elem.attr.emplace(VER, verValue);

    index = 6;
    if (checkClosing(tokens, index)) {
        return index + 2;
    }

    if (!checkPair(tokens, index, ENC)) {
        return std::nullopt;
    }
    elem.attr.emplace(ENC, std::get<std::string>(tokens.at(index + 2).data));

    index = 9;
    if (checkClosing(tokens, index)) {
        return index + 2;
    }

    if (!checkPair(tokens, index, STA)) {
        return std::nullopt;
    }
    const auto& staValue = std::get<std::string>(tokens.at(index + 2).data);
    if (staValue != "yes" && staValue != "no") {
        return std::nullopt;
    }
    elem.attr.emplace(STA, std::get<std::string>(tokens.at(index + 2).data));

    index = 12;
    if (checkClosing(tokens, index)) {
        return index + 2;
    }
    return std::nullopt;
}



