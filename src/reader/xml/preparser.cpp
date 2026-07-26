
#include "preparser.h"

#include <format>

#include "errorCode.h"
#include "log/ErrorStorage.h"
#include "reader/utilsReader.h"


using namespace xml;

std::unique_ptr<std::vector<Token>> Preparser::parseXML(const std::string& xml)
{
    tokens = std::make_unique<std::vector<Token>>();
    tokens->reserve(100);

    for (size_t index = 0; index < xml.length(); index++)
    {
        char symbol = xml[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            uint shift = parseString(xml, index);
            if (shift == 0) {
                return nullptr;
            }
            tokens->emplace_back(TokenType::DATA_STR_QUOTA, xml.substr(index + 1, shift - 1));
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            index = parseNumber<Token, TokenType>(xml, index, *tokens);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        size_t shift = parseStringOutQuotation(xml, index);
        if (shift < 0) {
            ErrorStorage::putError(ErrorCode::XML_PREPARSER_STRING_ERROR);
            return nullptr;
        }
        index += shift;
        continue;
    }
    tokens->shrink_to_fit();
    return std::move(tokens);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

size_t Preparser::parseStringOutQuotation(const std::string& json, uint index)
{
    uint shift = 0;
    while (index + shift < json.length()) {
        if (json[index + shift] == ' ' || tokensMap.contains(json[index + shift])) {
            tokens->emplace_back(TokenType::DATA_STR, json.substr(index, shift));
            return shift - 1;
        }
        shift += 1;
    }
    return -1;
}

