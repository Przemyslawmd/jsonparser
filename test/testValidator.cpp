
#include <fstream>

#include "../src/Preparser.h"
#include "../src/Validator.h"
#include "config.h"

#include <gtest/gtest.h>


TEST(ValidatorTest, FirstTest)
{
    std::string filePath = std::string(TEST_DATA_IMPROPER) + "/test_improper_begin_json.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
    std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);
    auto error = Validator().validate(tokens.get());

    ASSERT_EQ(error, ParseError::FIRST_CHAR_NOT_CURLY_OPEN);

}


TEST(ValidatorTest, SecondTest)
{
    std::string filePath = std::string(TEST_DATA_IMPROPER) + "/test_improper_end_json.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
        std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonStr);
    auto error = Validator().validate(tokens.get());

    ASSERT_EQ(error, ParseError::LAST_CHAR_NOT_CURLY_CLOSE);
}

