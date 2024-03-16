
#include "../src/Preparser.h"
#include "config.h"

#include <fstream>
#include <memory>
#include <vector>

#include <gtest/gtest.h>


struct TestData 
{
    TokenType type;
    std::variant<std::string, int, nullptr_t> data;
};


void checkTokens(std::unique_ptr<std::vector<PreToken>> tokens, std::vector<TestData>& testData)
{
    ASSERT_EQ(tokens->size(), testData.size());
    
    for (int i = 0; i < tokens->size(); i++) {
        ASSERT_EQ(tokens->at(i).type, testData[i].type );
        if (tokens->at(i).type == TokenType::DATA_INT) {
            ASSERT_EQ(std::get<int>(tokens->at(i).data), std::get<int>(testData[i].data));
        }
        else if (tokens->at(i).type == TokenType::DATA_STR) {
            ASSERT_EQ(std::get<std::string>(tokens->at(i).data), std::get<std::string>(testData[i].data));
        }
    }
}


TEST (PreparserTest, FirstTest)
{
    std::string filePath = std::string(TEST_DATA) + "/test_1.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
    std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN, nullptr },
       { TokenType::DATA_STR, std::string{ "person" }},
       { TokenType::COLON, nullptr } ,
       { TokenType::CURLY_OPEN, nullptr }, 
       { TokenType::DATA_STR, std::string{ "name" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "John" }},
       { TokenType::COMMA },
       { TokenType::DATA_STR, std::string{ "age" }},
       { TokenType::COLON} ,
       { TokenType::DATA_INT, 39 },
       { TokenType::COMMA },
       { TokenType::DATA_STR, std::string{ "country" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string{ "Poland" }},
       { TokenType::CURLY_CLOSE },
       { TokenType::CURLY_CLOSE },
    };

    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, SecondTest)
{
    std::string filePath = std::string(TEST_DATA) + "/test_2.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
    std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);

    std::vector<TestData> testData = {
       { TokenType::CURLY_OPEN, nullptr },
       
       { TokenType::DATA_STR, std::string{ "name" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "John Smith" }},
       { TokenType::COMMA }, //5
       
       { TokenType::DATA_STR, std::string{ "value" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "20223" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "price" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_INT, 2224 },
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "shipTo" }},
       { TokenType::COLON, nullptr },
       { TokenType::CURLY_OPEN, nullptr },

       { TokenType::DATA_STR, std::string{ "name" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "Jane Smith" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "address" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "123 Maple Street" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "city" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "Pretendville" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "state" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "NY" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "zip" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_INT, 12345},

       { TokenType::CURLY_CLOSE, nullptr },
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "billTo" }},
       { TokenType::COLON, nullptr },
       { TokenType::CURLY_OPEN, nullptr },

       { TokenType::DATA_STR, std::string{ "name" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "John Smith" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "address" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "123 Maple Street" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "city" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "Pretendville" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "state" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_STR, std::string{ "NY" }},
       { TokenType::COMMA },

       { TokenType::DATA_STR, std::string{ "zip" }},
       { TokenType::COLON, nullptr },
       { TokenType::DATA_INT, 12345},

       { TokenType::CURLY_CLOSE, nullptr },
       { TokenType::CURLY_CLOSE, nullptr },
    };

    checkTokens(std::move(tokens), testData);
}


TEST(PreparserTest, FirstImproperDataTest)
{
    std::string filePath = std::string(TEST_DATA_IMPROPER) + "/test_string_not_ended_1.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
    std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);

    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(preparser->getError(), ParseError::STRING_NOT_ENDED);
}


TEST(PreparserTest, SecondImproperDataTest)
{
    std::string filePath = std::string(TEST_DATA_IMPROPER) + "/test_string_not_ended_2.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
        std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);

    ASSERT_EQ(tokens, nullptr);
    ASSERT_EQ(preparser->getError(), ParseError::STRING_NOT_ENDED);
}

