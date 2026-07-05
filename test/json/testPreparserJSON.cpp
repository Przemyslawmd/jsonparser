
#include <chrono>
#include <memory>
#include <ranges>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

#include "log/ErrorStorage.h"
#include "reader/json/parserKey.h"
#include "reader/json/preparser.h"

#include "baseTest.h"
#include "config.h"
#include "utilsTest.h"


using namespace json;
using enum TokenType;

class TestPreparserJSON : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Token>> createTokens(const std::string& path, const std::string& file)
    {
        ErrorStorage::clear();
        std::string jsonString = getContentFromFile(path, file);

        const auto begin = std::chrono::high_resolution_clock::now();
        auto preparser = std::make_unique<Preparser>();
        auto tokens = preparser->parseJSON(jsonString);
        if (tokens == nullptr) {
            return nullptr;
        }
        tokens = createKeyTokens(std::move(tokens));

        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
        return tokens;
    }
};


void checkTokens(std::unique_ptr<std::vector<Token>> tokens, std::vector<Token>&& expected)
{
    ASSERT_EQ(tokens->size(), expected.size());

    auto compareData = []<typename T>(Token& token_1, Token& token_2)
    {
        if constexpr (std::is_same_v<T, double>) {
            ASSERT_TRUE((std::get<T>(token_1.data) - std::get<T>(token_2.data)) <= DBL_EPSILON);
        }
        else {
            ASSERT_EQ(std::get<T>(token_1.data), std::get<T>(token_2.data));
        }
    };

    using enum TokenType;

    for (const auto [token_1, token_2] : std::views::zip(*tokens, expected)) {
        ASSERT_EQ(token_1.type, token_2.type );
        switch (token_1.type) {
            case DATA_INT:
                compareData.template operator()<int64_t>(token_1, token_2);
                break;
            case DATA_STR:
            case KEY:
                compareData.template operator()<std::string>(token_1, token_2);
                break;
            case DATA_BOOL:
                compareData.template operator()<bool>(token_1, token_2);
                break;
            case DATA_DOUBLE:
                compareData.template operator()<double>(token_1, token_2);
                break;
        }
    }
}


TEST_F(TestPreparserJSON, Test_File_1)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_1.json");

    std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, std::string{ "person" }},
       { COLON } ,
       { CURLY_OPEN },
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "John" }},
       { COMMA },
       { KEY, std::string{ "age" }},
       { COLON } ,
       { DATA_INT, 39 },
       { COMMA },
       { KEY, std::string{ "country" }},
       { COLON },
       { DATA_STR, std::string{ "Poland" }},
       { COMMA },
       { KEY, std::string{ "employed" }},
       { COLON },
       { DATA_BOOL, true },
       { COMMA },
       { KEY, std::string{ "restricted" }},
       { COLON },
       { DATA_BOOL, false },
       { COMMA },
       { KEY, std::string{ "empty" }},
       { COLON },
       { DATA_NULL, nullptr },
       { CURLY_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), std::move(expected));
}


TEST_F(TestPreparserJSON, Test_File_2)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_2.json");

    std::vector<Token> expected = {
       { CURLY_OPEN },
       
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "John Smith" }},
       { COMMA },
       
       { KEY, std::string{ "value" }},
       { COLON },
       { DATA_STR, std::string{ "20223" }},
       { COMMA },

       { KEY, std::string{ "price" }},
       { COLON },
       { DATA_INT, 2224 },
       { COMMA },

       { KEY, std::string{ "shipTo" }},
       { COLON },
       { CURLY_OPEN },

       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "Jane Smith" }},
       { COMMA },

       { KEY, std::string{ "address" }},
       { COLON },
       { DATA_STR, std::string{ "123 Maple Street" }},
       { COMMA },

       { KEY, std::string{ "cities" }},
       { COLON },
       { SQUARE_OPEN },
       { DATA_STR, std::string{ "Pretendville" }},
       { COMMA },
       { DATA_STR, std::string{ "New York" }},
       { COMMA },
       { DATA_STR, std::string{ "Chicago" }},
       { SQUARE_CLOSE },
       { COMMA },

       { KEY, std::string{ "state" }},
       { COLON },
       { DATA_STR, std::string{ "NY" }},
       { COMMA },

       { KEY, std::string{ "zip" }},
       { COLON },
       { DATA_INT, -12345 },
       { COMMA },

       { KEY, std::string{ "weight" }},
       { COLON },
       { DATA_DOUBLE, 12.34 },

       { CURLY_CLOSE },
       { COMMA },

       { KEY, std::string{ "billTo" }},
       { COLON },
       { CURLY_OPEN },

       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "John Smith" }},
       { COMMA },

       { KEY, std::string{ "address" }},
       { COLON },
       { DATA_STR, std::string{ "123 Maple Street" }},
       { COMMA },

       { KEY, std::string{ "city" }},
       { COLON },
       { DATA_STR, std::string{ "Pretendville" }},
       { COMMA },

       { KEY, std::string{ "state" }},
       { COLON },
       { DATA_STR, std::string{ "NY" }},
       { COMMA },

       { KEY, std::string{ "zip" }},
       { COLON },
       { DATA_INT, 26111474836476 },
       { COMMA },

       { KEY, std::string{ "minusWeight" }},
       { COLON },
       { DATA_DOUBLE, -0.2456 },

       { CURLY_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), std::move(expected));
}


TEST_F(TestPreparserJSON, Test_File_6)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_6.json");

    std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, std::string{ "employees" }},
       { COLON } ,
       { SQUARE_OPEN },

       { CURLY_OPEN },
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "Agata" }},
       { COMMA },
       { KEY, std::string{ "email" }},
       { COLON } ,
       { DATA_STR, std::string{ "agata@gmail.com" }},
       { COMMA },
       { KEY, std::string{ "age" }},
       { COLON },
       { DATA_INT, 33 },
       { CURLY_CLOSE },
       { COMMA },

       { CURLY_OPEN },
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "Anna" }},
       { COMMA },
       { KEY, std::string{ "email" }},
       { COLON } ,
       { DATA_STR, std::string{ "anna@gmail.com" }},
       { COMMA },
       { KEY, std::string{ "age" }},
       { COLON },
       { DATA_INT, 31 },

       { CURLY_CLOSE },
       { SQUARE_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), std::move(expected));
}


TEST_F(TestPreparserJSON, Test_File_7)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_7.json");

    std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, std::string{ "employees" }},
       { COLON },
       { SQUARE_OPEN },

       { CURLY_OPEN },
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "Agata" }},
       { COMMA },
       { KEY, std::string{ "data" }},
       { COLON },
       { SQUARE_OPEN },
       { SQUARE_OPEN },
       { DATA_INT, 1 },
       { COMMA },
       { DATA_INT, 2 },
       { COMMA },
       { DATA_INT, 3 },
       { SQUARE_CLOSE },
       { COMMA },
       { SQUARE_OPEN },
       { DATA_INT, 4 },
       { COMMA },
       { DATA_INT, 5 },
       { COMMA },
       { DATA_INT, 6 },
       { SQUARE_CLOSE },
       { SQUARE_CLOSE },
       { CURLY_CLOSE },
       { COMMA },

       { CURLY_OPEN },
       { KEY, std::string{ "name" }},
       { COLON },
       { DATA_STR, std::string{ "Anna" }},
       { COMMA },
       { KEY, std::string{ "data" }},
       { COLON },
       { SQUARE_OPEN },
       { SQUARE_OPEN },
       { DATA_STR, "a" },
       { COMMA },
       { DATA_STR, "b" },
       { SQUARE_CLOSE },
       { COMMA },
       { SQUARE_OPEN },
       { DATA_STR, "c d e" },
       { SQUARE_CLOSE },
       { SQUARE_CLOSE },
       { CURLY_CLOSE },
       { SQUARE_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), std::move(expected));
}


TEST_F(TestPreparserJSON, Test_File_8)
{
    auto tokens = createTokens(TEST_DATA, "test_8_complex.json");
    ASSERT_TRUE(tokens != nullptr);
}


TEST_F(TestPreparserJSON, FirstImproperDataTest)
{
    auto tokens = createTokens(TEST_DATA_IMPROPER_JSON, "string_not_ended_1.json");

    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::JSON_PREPARSER_UNKNOWN_SYMBOL);
}


TEST_F(TestPreparserJSON, SecondImproperDataTest)
{
    auto tokens = createTokens(TEST_DATA_IMPROPER_JSON, "string_not_ended_2.json");

    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::PREPARSER_STRING_ERROR);
}


TEST_F(TestPreparserJSON, UnknownSymbol_1)
{
    auto tokens = createTokens(TEST_DATA_IMPROPER_JSON, "preparser_unknown_symbol_1.json");

    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::JSON_PREPARSER_UNKNOWN_SYMBOL);
}


TEST_F(TestPreparserJSON, UnknownSymbol_2)
{
    auto tokens = createTokens(TEST_DATA_IMPROPER_JSON, "preparser_unknown_symbol_2.json");

    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::JSON_PREPARSER_UNKNOWN_SYMBOL);
}


TEST_F(TestPreparserJSON, UnknownSymbol_3)
{
    auto tokens = createTokens(TEST_DATA_IMPROPER_JSON, "preparser_unknown_symbol_3.json");

    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::JSON_PREPARSER_UNKNOWN_SYMBOL);
}

