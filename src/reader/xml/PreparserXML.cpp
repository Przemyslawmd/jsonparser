
#include "PreparserXML.h"

#include <format>

#include "error.h"
#include "log/ErrorStorage.h"


std::unique_ptr<std::vector<TokenXML>> PreparserXML::parseXML(const std::string& xml)
{
    tokens = std::make_unique<std::vector<TokenXML>>();
    tokens->reserve(100);

    for (size_t index = 0; index < xml.length(); index++)
    {
        char symbol = xml[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            size_t shift = parseStringInQuotation(xml, index);
            if (shift == 0) {
                return nullptr;
            }
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            index = parseNumber(xml, index);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        size_t shift = parseStringOutQuotation(xml, index);
        if (shift == 0) {
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

size_t PreparserXML::parseNumber(const std::string& json, size_t index)
{
    int64_t number = 0;
    bool isMinus = false;

    if (isdigit(json[index])) {
        number = json[index] - '0';
    }
    else {
        isMinus = true;
    }

    index++;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + json[index] - '0';
        index++;
    }
    if (json[index] != '.') {
        tokens->emplace_back(TokenTypeXML::DATA_INT, isMinus ? number * -1 : number);
        return index - 1;
    }

    index++;
    size_t divider = 1;
    while (index < json.length() && isdigit(json[index])) {
        number = number * 10 + (json[index] - '0');
        index++;
        divider *= 10;
    }
    double numberFloat = (double) number / divider;
    tokens->emplace_back(TokenTypeXML::DATA_DOUBLE, isMinus ? numberFloat * -1.0 : numberFloat);
    return index - 1;
}


size_t PreparserXML::parseStringInQuotation(const std::string& json, size_t index)
{
    size_t shift = 1;
    while (index + shift < json.length()) {
        if (json[index + shift] == '\"') {
            tokens->emplace_back(TokenTypeXML::DATA_STR_QUOTA, json.substr(index + 1, shift - 1));
            return shift;
        }
        shift += 1;
    }
    return 0;
}


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
    return 0;
}

