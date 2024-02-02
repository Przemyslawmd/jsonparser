
#include "../src/Preparser.h"
#include "config.h"

#include <fstream>
#include <memory>
#include <vector>

#include <gtest/gtest.h>


struct TestData 
{
    TokenType type;
    std::variant<std::string, int, float, bool, nullptr_t> data;
};


void checkTokens(std::unique_ptr<std::vector<Token>> tokens, std::vector<TestData>& testData)
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
    std::string filePath = std::string(TEST_DATA) + "/test.json";
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
       { TokenType::DATA_STR, std::string {"country" }},
       { TokenType::COLON },
       { TokenType::DATA_STR, std::string {"Poland" }},
       { TokenType::CURLY_CLOSE },
       { TokenType::CURLY_CLOSE },
    };

    checkTokens(std::move(tokens), testData);
}

