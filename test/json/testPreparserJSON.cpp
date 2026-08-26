
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
#include "paths.h"
#include "utilsTest.h"


using namespace json;
using enum TokenType;


namespace
{
class TestPreparserJSON : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Token>> createTokens(const std::string& path, const std::string& file)
    {
        ErrorStorage::clear();
        const std::string jsonString = getContentFromFile(path, file);

        const auto preparser = std::make_unique<Preparser>();
        auto tokens = preparser->parseJSON(jsonString);
        if (!tokens) {
            return nullptr;
        }
        createKeyTokens(*tokens);
        return tokens;
    }

    void testPerformance(const std::string& path, const std::string& file)
    {
        const std::string jsonString = getContentFromFile(path, file);
        const auto preparser = std::make_unique<Preparser>();

        const auto begin = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < 100; i++) {
            const auto tokens = preparser->parseJSON(jsonString);
            createKeyTokens(*tokens);
        }
        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
    }
};
}


static void checkTokens(const std::vector<Token>& tokens, const std::vector<Token>& expected)
{
    ASSERT_EQ(tokens.size(), expected.size());

    auto compareData = []<typename T>(const Token& token_1, const Token& token_2)
    {
        if constexpr (std::is_same_v<T, double>) {
            ASSERT_TRUE((std::get<T>(token_1.data) - std::get<T>(token_2.data)) <= DBL_EPSILON);
        }
        else {
            ASSERT_EQ(std::get<T>(token_1.data), std::get<T>(token_2.data));
        }
    };

    for (const auto& [token_1, token_2] : std::views::zip(tokens, expected)) {
        ASSERT_EQ(token_1.type, token_2.type);
        switch (token_1.type) {
            case DATA_INT:
                compareData.operator()<int64_t>(token_1, token_2);
                break;
            case DATA_STR:
            case KEY:
                compareData.operator()<std::string>(token_1, token_2);
                break;
            case DATA_BOOL:
                compareData.operator()<bool>(token_1, token_2);
                break;
            case DATA_DOUBLE:
                compareData.operator()<double>(token_1, token_2);
                break;
        }
    }
}


TEST_F(TestPreparserJSON, Test_File_1)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_1.json");

    const std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, { "person" }},
       { COLON } ,
       { CURLY_OPEN },
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "John" }},
       { COMMA },
       { KEY, { "age" }},
       { COLON } ,
       { DATA_INT, 39 },
       { COMMA },
       { KEY, { "country" }},
       { COLON },
       { DATA_STR, { "Poland" }},
       { COMMA },
       { KEY, { "employed" }},
       { COLON },
       { DATA_BOOL, true },
       { COMMA },
       { KEY, { "restricted" }},
       { COLON },
       { DATA_BOOL, false },
       { COMMA },
       { KEY, { "empty" }},
       { COLON },
       { DATA_NULL, nullptr },
       { CURLY_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(*tokens, expected);
}


TEST_F(TestPreparserJSON, Test_File_2)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_2.json");

    const std::vector<Token> expected = {
       { CURLY_OPEN },
       
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "John Smith" }},
       { COMMA },
       
       { KEY, { "value" }},
       { COLON },
       { DATA_STR, { "20223" }},
       { COMMA },

       { KEY, { "price" }},
       { COLON },
       { DATA_INT, 2224 },
       { COMMA },

       { KEY, { "shipTo" }},
       { COLON },
       { CURLY_OPEN },

       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "Jane Smith" }},
       { COMMA },

       { KEY, { "address" }},
       { COLON },
       { DATA_STR, { "123 Maple Street" }},
       { COMMA },

       { KEY, { "cities" }},
       { COLON },
       { SQUARE_OPEN },
       { DATA_STR, { "Pretendville" }},
       { COMMA },
       { DATA_STR, { "New York" }},
       { COMMA },
       { DATA_STR, { "Chicago" }},
       { SQUARE_CLOSE },
       { COMMA },

       { KEY, { "state" }},
       { COLON },
       { DATA_STR, { "NY" }},
       { COMMA },

       { KEY, { "zip" }},
       { COLON },
       { DATA_INT, -12345 },
       { COMMA },

       { KEY, { "weight" }},
       { COLON },
       { DATA_DOUBLE, 12.34 },

       { CURLY_CLOSE },
       { COMMA },

       { KEY, { "billTo" }},
       { COLON },
       { CURLY_OPEN },

       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "John Smith" }},
       { COMMA },

       { KEY, { "address" }},
       { COLON },
       { DATA_STR, { "123 Maple Street" }},
       { COMMA },

       { KEY, { "city" }},
       { COLON },
       { DATA_STR, { "Pretendville" }},
       { COMMA },

       { KEY, { "state" }},
       { COLON },
       { DATA_STR, { "NY" }},
       { COMMA },

       { KEY, { "zip" }},
       { COLON },
       { DATA_INT, 26111474836476 },
       { COMMA },

       { KEY, { "minusWeight" }},
       { COLON },
       { DATA_DOUBLE, -0.2456 },

       { CURLY_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(*tokens, expected);
}


TEST_F(TestPreparserJSON, Test_File_6)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_6.json");

    const std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, { "employees" }},
       { COLON } ,
       { SQUARE_OPEN },

       { CURLY_OPEN },
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "Agata" }},
       { COMMA },
       { KEY, { "email" }},
       { COLON } ,
       { DATA_STR, { "agata@gmail.com" }},
       { COMMA },
       { KEY, { "age" }},
       { COLON },
       { DATA_INT, 33 },
       { CURLY_CLOSE },
       { COMMA },

       { CURLY_OPEN },
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "Anna" }},
       { COMMA },
       { KEY, { "email" }},
       { COLON } ,
       { DATA_STR, { "anna@gmail.com" }},
       { COMMA },
       { KEY, { "age" }},
       { COLON },
       { DATA_INT, 31 },

       { CURLY_CLOSE },
       { SQUARE_CLOSE },
       { CURLY_CLOSE },
    };
    checkTokens(*tokens, expected);
}


TEST_F(TestPreparserJSON, Test_File_7)
{
    auto tokens = createTokens(TEST_DATA_JSON, "test_7.json");

    const std::vector<Token> expected = {
       { CURLY_OPEN },
       { KEY, { "employees" }},
       { COLON },
       { SQUARE_OPEN },

       { CURLY_OPEN },
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "Agata" }},
       { COMMA },
       { KEY, { "data" }},
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
       { KEY, { "name" }},
       { COLON },
       { DATA_STR, { "Anna" }},
       { COMMA },
       { KEY, { "data" }},
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
    checkTokens(*tokens, expected);
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


TEST_F(TestPreparserJSON, Performance)
{
    checkDuration = true;
    testPerformance(TEST_DATA_JSON, "test_8_complex.json");
}

