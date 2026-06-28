
#include "preparserXML.h"

#include <format>

#include "error.h"
#include "log/ErrorStorage.h"
#include "../utilsReader.h"


using namespace xml;

std::unique_ptr<std::vector<Token>> PreparserXML::parseXML(const std::string& xml)
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
            size_t shift = parseString<Token,TokenTypeXML>(xml, index, *tokens, TokenTypeXML::DATA_STR_QUOTA);
            if (shift == 0) {
                return nullptr;
            }
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            index = parseNumber<Token, TokenTypeXML>(xml, index, *tokens);
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

size_t PreparserXML::parseStringOutQuotation(const std::string& json, size_t index)
{
    size_t shift = 0;
    while (index + shift < json.length()) {
        if (json[index + shift] == ' ' || tokensMap.contains(json[index + shift])) {
            tokens->emplace_back(TokenTypeXML::DATA_STR, json.substr(index, shift));
            return shift - 1;
        }
        shift += 1;
    }
    return -1;
}

