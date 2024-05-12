
#include <fstream>

#include <gtest/gtest.h>

#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "config.h"


Result getValidatorError(const std::string& jsonFile)
{
    std::string filePath = std::string(TEST_DATA_IMPROPER) + jsonFile;
    std::ifstream jsonStream(filePath);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    return Validator().validate(*tokens.get());
}


TEST(ValidatorTest, ImproperBegin)
{
    Result error = getValidatorError("improper_begin.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_BEGINNING);
}


TEST(ValidatorTest, ImproperEnd)
{
    Result error = getValidatorError("improper_end.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_END);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyOpen)
{
    Result error = getValidatorError("not_allowed_after_curly_open.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyClose)
{
    Result error = getValidatorError("not_allowed_after_curly_close.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterString)
{
    Result error = getValidatorError("not_allowed_after_string.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING);
}


TEST(ValidatorTest, ImproperTokenAfterInt)
{
    Result error = getValidatorError("not_allowed_after_int.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT);
}


TEST(ValidatorTest, ImproperTokenAfterDouble)
{
    Result error = getValidatorError("not_allowed_after_double.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE);
}


TEST(ValidatorTest, ImproperTokenAfterBool)
{
    Result error = getValidatorError("not_allowed_after_bool.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL);
}


TEST(ValidatorTest, ImproperTokenAfterColon)
{
    Result error = getValidatorError("not_allowed_after_colon.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON);
}


TEST(ValidatorTest, ImproperTokenAfterComma)
{
    Result error = getValidatorError("not_allowed_after_comma.json");
    ASSERT_EQ(error, Result::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA);
}

