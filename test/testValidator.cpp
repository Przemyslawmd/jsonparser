
#include <fstream>

#include <gtest/gtest.h>

#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "config.h"
#include "utils.h"


std::unique_ptr<Error> getValidatorError(const std::string& jsonFile)
{
    Utils utils;
    std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA_IMPROPER), jsonFile);
    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    Validator validator;
    bool result = validator.validate(*tokens.get());
    EXPECT_FALSE(result);
    return validator.getError();
}


TEST(ValidatorTest, ImproperBegin)
{
    auto error = getValidatorError("improper_begin.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_BEGINNING);
}


TEST(ValidatorTest, ImproperEnd)
{
    auto error = getValidatorError("improper_end.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_END);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyOpen)
{
    auto error = getValidatorError("not_allowed_after_curly_open.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyClose)
{
    auto error = getValidatorError("not_allowed_after_curly_close.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterString)
{
    auto error = getValidatorError("not_allowed_after_string.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_STRING);
}


TEST(ValidatorTest, ImproperTokenAfterInt)
{
    auto error = getValidatorError("not_allowed_after_int.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_INT);
}


TEST(ValidatorTest, ImproperTokenAfterDouble)
{
    auto error = getValidatorError("not_allowed_after_double.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_DOUBLE);
}


TEST(ValidatorTest, ImproperTokenAfterBool)
{
    auto error = getValidatorError("not_allowed_after_bool.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_DATA_BOOL);
}


TEST(ValidatorTest, ImproperTokenAfterColon)
{
    auto error = getValidatorError("not_allowed_after_colon.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COLON);
}


TEST(ValidatorTest, ImproperTokenAfterComma)
{
    auto error = getValidatorError("not_allowed_after_comma.json");
    ASSERT_EQ(error->getErrorCode(), ErrorCode::VALIDATOR_IMPROPER_TOKEN_AFTER_COMMA);
}

