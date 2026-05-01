
#include "Preparser.h"

#include <format>

#include "error.h"
#include "log/ErrorStorage.h"
#include "utilsReader.h"


constexpr size_t FALSE_WORD_LEN = 5;
constexpr size_t TRUE_WORD_LEN = 4;
constexpr size_t NULL_WORD_LEN = 4;


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    tokens = std::make_unique<std::vector<Token>>();
    tokens->reserve(100);

    for (size_t index = 0; index < json.length(); index++)
    {
        char symbol = json[index];
        if (symbol == ' ' || symbol == '\n') {
            continue;
        }
        if (symbol == '\"') {
            size_t shift = parseString<Token, TokenType>(json, index, *tokens, TokenType::DATA_STR);
            if (shift == 0) {
                return nullptr;
            }
            index += shift;
            continue;
        }
        if (isdigit(symbol) || symbol == '-') {
            index = parseNumber<Token, TokenType>(json, index, *tokens);
            continue;
        }
        if (tokensMap.count(symbol)) {
            tokens->emplace_back(tokensMap.at(symbol), nullptr);
            continue;
        }
        if (symbol == 'f') {
            if (json.length() - index > FALSE_WORD_LEN && (json.compare(index, FALSE_WORD_LEN, "false") == 0)) {
                tokens->emplace_back(TokenType::DATA_BOOL, false);
                index += (FALSE_WORD_LEN - 1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 't') {
            if (json.length() - index > TRUE_WORD_LEN && (json.compare(index, TRUE_WORD_LEN, "true") == 0)) {
                tokens->emplace_back(TokenType::DATA_BOOL, true);
                index += (TRUE_WORD_LEN - 1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        if (symbol == 'n') {
            if (json.length() - index > NULL_WORD_LEN && (json.compare(index, NULL_WORD_LEN, "null") == 0)) {
                tokens->emplace_back(TokenType::DATA_NULL, nullptr);
                index += (NULL_WORD_LEN - 1);
                continue;
            }
            ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
            return nullptr;
        }
        ErrorStorage::putError(ErrorCode::PREPARSER_UNKNOWN_SYMBOL, std::format("Position at index {}", index));
        return nullptr;
    }
    tokens->shrink_to_fit();
    return std::move(tokens);
}

