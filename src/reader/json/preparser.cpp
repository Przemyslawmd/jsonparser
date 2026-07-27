
#include "preparser.h"

#include <format>

#include "errorCode.h"
#include "log/ErrorStorage.h"
#include "reader/utilsReader.h"


using namespace json;

std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    using enum ErrorCode;
    using enum TokenType;

    auto checkWord = [](const std::string& json, const std::string& soughtWord, size_t& index) -> bool
    {
        if (json.length() - index > soughtWord.length() && (json.compare(index, soughtWord.length(), soughtWord) == 0)) {
            index += (soughtWord.length() - 1);
            return true;
        }
        ErrorStorage::putError(JSON_PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
        return false;
    };

    tokens = std::make_unique<std::vector<Token>>();
    tokens->reserve(100);

    for (size_t index = 0; index < json.length(); index++)
    {
        char symbol = json[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            size_t shift = parseString(json, index);
            if (shift == 0) {
                return nullptr;
            }
            tokens->emplace_back(DATA_STR, json.substr(index + 1, shift - 1));
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            auto[newIndex, num] = parseNumber(json, index);
            if (std::holds_alternative<int64_t>(num)) {
                tokens->emplace_back(DATA_INT, std::get<int64_t>(num));
            }
            else {
                tokens->emplace_back(DATA_DOUBLE, std::get<double>(num));
            }
            index = newIndex;
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        if (symbol == 'f') {
            if (checkWord(json, "false", index)) {
                tokens->emplace_back(DATA_BOOL, false);
                continue;
            }
            return nullptr;
        }
        if (symbol == 't') {
            if (checkWord(json, "true", index)) {
                tokens->emplace_back(DATA_BOOL, true);
                continue;
            }
            return nullptr;
        }
        if (symbol == 'n') {
            if (checkWord(json, "null", index)) {
                tokens->emplace_back(DATA_NULL, nullptr);
                continue;
            }
            return nullptr;
        }
        ErrorStorage::putError(JSON_PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
        return nullptr;
    }
    tokens->shrink_to_fit();
    return std::move(tokens);
}

