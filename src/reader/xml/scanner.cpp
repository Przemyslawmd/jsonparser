
#include "scanner.h"

#include <format>
#include <map>

#include "errorCode.h"
#include "log/ErrorStorage.h"
#include "reader/utilsReader.h"


namespace
{
    const std::map<char, xml::TokenType> tokensMap
    {
        { '=', xml::TokenType::EQUAL },
        { '?', xml::TokenType::QUESTION },
        { '/', xml::TokenType::SLASH },
        { '<', xml::TokenType::ANGLE_OPEN },
        { '>', xml::TokenType::ANGLE_CLOSE },
    };

    int parseStringNoQuotation(const std::string_view xml, const unsigned int index)
    {
        int shift = 0;
        while (index + shift < xml.length()) {
            if (xml[index + shift] == ' ' || tokensMap.contains(xml[index + shift])) {
                return shift - 1;
            }
            shift += 1;
        }
        ErrorStorage::putError(ErrorCode::XML_PREPARSER_STRING_ERROR);
        return -1;
    }
}


namespace xml
{

std::unique_ptr<std::vector<Token>> scanString(const std::string_view xml)
{
    using enum TokenType;

    auto tokens = std::make_unique<std::vector<Token>>();
    tokens->reserve(500);

    for (size_t index = 0; index < xml.length(); index++)
    {
        char symbol = xml[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            const unsigned int shift = parseString(xml, index);
            if (shift == 0) {
                return nullptr;
            }
            tokens->emplace_back(DATA_STR_QUOTA, std::string(xml.data() + index + 1, shift - 1));
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            auto [newIndex, num] = parseNumber(xml, index);
            if (std::holds_alternative<int64_t>(num)) {
                tokens->emplace_back(DATA_INT, std::get<int64_t>(num));
            }
            else {
                tokens->emplace_back(DATA_DOUBLE, std::get<double>(num));
            }
            index = newIndex;
            continue;
        }
        if (tokensMap.contains(symbol)) {
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        const int shift = parseStringNoQuotation(xml, index);
        if (shift < 0) {
            return nullptr;
        }
        tokens->emplace_back(DATA_STR, std::string(xml.data() + index, shift + 1));
        index += shift;
    }
    tokens->shrink_to_fit();
    return std::move(tokens);
}
}

