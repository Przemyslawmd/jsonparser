
#include <fstream>

#include <gtest/gtest.h>

#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "config.h"
#include "utils.h"


ErrorCode getValidatorError(const std::string& jsonFile)
{
    Utils utils;
    std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA_IMPROPER), jsonFile);

    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    return Validator().validate(*tokens.get());
}


TEST(ValidatorTest, ImproperBegin)
{
    ErrorCode error = getValidatorError("improper_begin.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_BEGINNING);
}


TEST(ValidatorTest, ImproperEnd)
{
    ErrorCode error = getValidatorError("improper_end.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_END);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyOpen)
{
    ErrorCode error = getValidatorError("not_allowed_after_curly_open.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyClose)
{
    ErrorCode error = getValidatorError("not_allowed_after_curly_close.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterString)
{
    ErrorCode error = getValidatorError("not_allowed_after_string.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING);
}


TEST(ValidatorTest, ImproperTokenAfterInt)
{
    ErrorCode error = getValidatorError("not_allowed_after_int.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT);
}


TEST(ValidatorTest, ImproperTokenAfterDouble)
{
    ErrorCode error = getValidatorError("not_allowed_after_double.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE);
}


TEST(ValidatorTest, ImproperTokenAfterBool)
{
    ErrorCode error = getValidatorError("not_allowed_after_bool.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL);
}


TEST(ValidatorTest, ImproperTokenAfterColon)
{
    ErrorCode error = getValidatorError("not_allowed_after_colon.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON);
}


TEST(ValidatorTest, ImproperTokenAfterComma)
{
    ErrorCode error = getValidatorError("not_allowed_after_comma.json");
    ASSERT_EQ(error, ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA);
}

