
#include <fstream>

#include <gtest/gtest.h>

#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/log/ErrorStorage.h"
#include "config.h"
#include "utils.h"


void makeValidatorError(const std::string& jsonFile)
{
    ErrorStorage::clear();
    std::string jsonString = getJsonFromFile(TEST_DATA_IMPROPER, jsonFile);
    auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    Validator validator;
    bool result = validator.validate(*tokens);
    EXPECT_FALSE(result);
}


TEST(ValidatorTest, ImproperBegin)
{
    makeValidatorError("improper_begin.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_IMPROPER_BEGIN);
}


TEST(ValidatorTest, ImproperEnd)
{
    makeValidatorError("improper_end.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_IMPROPER_END);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyOpen)
{
    makeValidatorError("not_allowed_after_curly_open.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyClose)
{
    makeValidatorError("not_allowed_after_curly_close.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterString)
{
    makeValidatorError("not_allowed_after_string.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_STRING);
}


TEST(ValidatorTest, ImproperTokenAfterInt)
{
    makeValidatorError("not_allowed_after_int.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_INT);
}


TEST(ValidatorTest, ImproperTokenAfterDouble)
{
    makeValidatorError("not_allowed_after_double.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_DOUBLE);
}


TEST(ValidatorTest, ImproperTokenAfterBool)
{
    makeValidatorError("not_allowed_after_bool.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_BOOL);
}


TEST(ValidatorTest, ImproperTokenAfterColon)
{
    makeValidatorError("not_allowed_after_colon.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_COLON);
}


TEST(ValidatorTest, ImproperTokenAfterComma)
{
    makeValidatorError("not_allowed_after_comma.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::VALIDATOR_TOKEN_AFTER_COMMA);
}

