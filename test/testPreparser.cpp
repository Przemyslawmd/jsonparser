
#include <chrono>
#include <fstream>
#include <limits>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "config.h"


constexpr bool measurement = true;


struct TestData 
{
    TokenType type;
    std::variant<std::string, int64_t, double, bool, nullptr_t> data;
};


std::unique_ptr<std::vector<Token>> getTokens(const std::string& path, const std::string& file, ErrorCode* error)
{
    std::ifstream jsonStream(path + file);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    auto begin = std::chrono::high_resolution_clock::now();
    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    std::unique_ptr<Error> errorPreparser = preparser->getError();
    if (errorPreparser != nullptr) {
        *error = errorPreparser->getErrorCode();
    }
    if (tokens == nullptr) {
        return nullptr;
    }
    tokens = createKeyTokens(std::move(tokens));

    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    return tokens;
}


void checkTokens(std::unique_ptr<std::vector<Token>> tokens, std::vector<TestData>& testData)
{
    ASSERT_EQ(tokens->size(), testData.size());
    
    for (int i = 0; i < tokens->size(); i++) {
        ASSERT_EQ(tokens->at(i).type, testData[i].type );
        if (tokens->at(i).type == TokenType::DATA_INT) {
            ASSERT_EQ(std::get<int64_t>(tokens->at(i).data), std::get<int64_t>(testData[i].data));
        }
        else if (tokens->at(i).type == TokenType::DATA_STR || tokens->at(i).type == TokenType::KEY) {
            ASSERT_EQ(std::get<std::string>(tokens->at(i).data), std::get<std::string>(testData[i].data));
        }
        else if (tokens->at(i).type == TokenType::DATA_BOOL) {
            ASSERT_EQ(std::get<bool>(tokens->at(i).data), std::get<bool>(testData[i].data));
        }
        else if (tokens->at(i).type == TokenType::DATA_DOUBLE) {
            ASSERT_TRUE((std::get<double>(tokens->at(i).data) - std::get<double>(testData[i].data)) <= DBL_EPSILON);
        }
    }
}


TEST (PreparserTest, Test_File_1)
{
    ErrorCode error;
    auto tokens = getTokens(TEST_DATA, "test_1.json", &error);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "person" }},
       { TokenType::COLON } ,
       { TokenType::CURLY_OPEN }, 
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "John" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "age" }},
       { TokenType::COLON } ,
       { TokenType::DATA_INT, 39 },
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "country" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Poland" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "employed" }},
       { TokenType::COLON },
       { TokenType::DATA_BOOL, true },
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "restricted" }},
       { TokenType::COLON },
       { TokenType::DATA_BOOL, false },
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "empty" }},
       { TokenType::COLON },
       { TokenType::DATA_NULL, nullptr },
       { TokenType::CURLY_CLOSE },
       { TokenType::CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, Test_File_2)
{
    ErrorCode error;
    auto tokens = getTokens(TEST_DATA, "test_2.json", &error);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN },
       
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "John Smith" }},
       { TokenType::COMMA },
       
       { TokenType::KEY, std::string{ "value" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "20223" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "price" }},
       { TokenType::COLON },
       { TokenType::DATA_INT, 2224 },
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "shipTo" }},
       { TokenType::COLON },
       { TokenType::CURLY_OPEN },

       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Jane Smith" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "address" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "123 Maple Street" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "cities" }},
       { TokenType::COLON },
       { TokenType::SQUARE_OPEN },
       { TokenType::DATA_STR, std::string{ "Pretendville" }},
       { TokenType::COMMA },
       { TokenType::DATA_STR, std::string{ "New York" }},
       { TokenType::COMMA },
       { TokenType::DATA_STR, std::string{ "Chicago" }},
       { TokenType::SQUARE_CLOSE },
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "state" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "NY" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "zip" }},
       { TokenType::COLON },
       { TokenType::DATA_INT, -12345 },
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "weight" }},
       { TokenType::COLON },
       { TokenType::DATA_DOUBLE, 12.34 },

       { TokenType::CURLY_CLOSE },
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "billTo" }},
       { TokenType::COLON },
       { TokenType::CURLY_OPEN },

       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "John Smith" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "address" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "123 Maple Street" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "city" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Pretendville" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "state" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "NY" }},
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "zip" }},
       { TokenType::COLON },
       { TokenType::DATA_INT, 26111474836476 },
       { TokenType::COMMA },

       { TokenType::KEY, std::string{ "minusWeight" }},
       { TokenType::COLON },
       { TokenType::DATA_DOUBLE, -0.2456 },

       { TokenType::CURLY_CLOSE },
       { TokenType::CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, Test_File_6)
{
    ErrorCode error;
    auto tokens = getTokens(TEST_DATA, "test_6.json", &error);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "employees" }},
       { TokenType::COLON } ,
       { TokenType::SQUARE_OPEN },

       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Agata" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "email" }},
       { TokenType::COLON } ,
       { TokenType::DATA_STR, std::string{ "agata@gmail.com" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "age" }},
       { TokenType::COLON },
       { TokenType::DATA_INT, 33 },
       { TokenType::CURLY_CLOSE },
       { TokenType::COMMA },

       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Anna" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "email" }},
       { TokenType::COLON } ,
       { TokenType::DATA_STR, std::string{ "anna@gmail.com" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "age" }},
       { TokenType::COLON },
       { TokenType::DATA_INT, 31 },

       { TokenType::CURLY_CLOSE },
       { TokenType::SQUARE_CLOSE },
       { TokenType::CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, Test_File_7)
{
    ErrorCode error;
    auto tokens = getTokens(TEST_DATA, "test_7.json", &error);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "employees" }},
       { TokenType::COLON },
       { TokenType::SQUARE_OPEN },

       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Agata" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "data" }},
       { TokenType::COLON },
       { TokenType::SQUARE_OPEN },
       { TokenType::SQUARE_OPEN },
       { TokenType::DATA_INT, 1 },
       { TokenType::COMMA },
       { TokenType::DATA_INT, 2 },
       { TokenType::COMMA },
       { TokenType::DATA_INT, 3 },
       { TokenType::SQUARE_CLOSE },
       { TokenType::COMMA },
       { TokenType::SQUARE_OPEN },
       { TokenType::DATA_INT, 4 },
       { TokenType::COMMA },
       { TokenType::DATA_INT, 5 },
       { TokenType::COMMA },
       { TokenType::DATA_INT, 6 },
       { TokenType::SQUARE_CLOSE },
       { TokenType::SQUARE_CLOSE },
       { TokenType::CURLY_CLOSE },
       { TokenType::COMMA },

       { TokenType::CURLY_OPEN },
       { TokenType::KEY, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Anna" }},
       { TokenType::COMMA },
       { TokenType::KEY, std::string{ "data" }},
       { TokenType::COLON },
       { TokenType::SQUARE_OPEN },
       { TokenType::SQUARE_OPEN },
       { TokenType::DATA_STR, "a" },
       { TokenType::COMMA },
       { TokenType::DATA_STR, "b" },
       { TokenType::SQUARE_CLOSE },
       { TokenType::COMMA },
       { TokenType::SQUARE_OPEN },
       { TokenType::DATA_STR, "c d e" },
       { TokenType::SQUARE_CLOSE },
       { TokenType::SQUARE_CLOSE },
       { TokenType::CURLY_CLOSE },
       { TokenType::SQUARE_CLOSE },
       { TokenType::CURLY_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, Test_File_8)
{
    ErrorCode error;
    auto tokens = getTokens(TEST_DATA, "test_8_complex.json", &error);
    ASSERT_TRUE(tokens != nullptr);
}


TEST(PreparserTest, FirstImproperDataTest)
{
    ErrorCode error = ErrorCode::NO_ERROR;
    auto tokens = getTokens(TEST_DATA_IMPROPER, "string_not_ended_1.json", &error);

    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(error, ErrorCode::PREPARSER_UNKNOWN_SYMBOL);
}


TEST(PreparserTest, SecondImproperDataTest)
{
    ErrorCode error = ErrorCode::NO_ERROR;
    auto tokens = getTokens(TEST_DATA_IMPROPER, "string_not_ended_2.json", &error);

    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(error, ErrorCode::PREPARSER_STRING_ERROR);
}

