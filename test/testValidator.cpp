
#include <fstream>

#include "../src/Preparser.h"
#include "../src/Validator.h"
#include "config.h"

#include <gtest/gtest.h>


ParseError getValidatorError(const std::string& jsonFile)
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
    ParseError error = getValidatorError("improper_begin.json");
    ASSERT_EQ(error, ParseError::FIRST_CHAR_NOT_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperEnd)
{
    ParseError error = getValidatorError("improper_end.json");
    ASSERT_EQ(error, ParseError::LAST_CHAR_NOT_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyOpen)
{
    ParseError error = getValidatorError("not_allowed_after_curly_open.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_CURLY_OPEN);
}


TEST(ValidatorTest, ImproperTokenAfterCurlyClose)
{
    ParseError error = getValidatorError("not_allowed_after_curly_close.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_CURLY_CLOSE);
}


TEST(ValidatorTest, ImproperTokenAfterString)
{
    ParseError error = getValidatorError("not_allowed_after_string.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_STRING);
}


TEST(ValidatorTest, ImproperTokenAfterInt)
{
    ParseError error = getValidatorError("not_allowed_after_int.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_DATA_INT);
}


TEST(ValidatorTest, ImproperTokenAfterDouble)
{
    ParseError error = getValidatorError("not_allowed_after_double.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_DATA_DOUBLE);
}


TEST(ValidatorTest, ImproperTokenAfterBool)
{
    ParseError error = getValidatorError("not_allowed_after_bool.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_DATA_BOOL);
}


TEST(ValidatorTest, ImproperTokenAfterColon)
{
    ParseError error = getValidatorError("not_allowed_after_colon.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_COLON);
}


TEST(ValidatorTest, ImproperTokenAfterComma)
{
    ParseError error = getValidatorError("not_allowed_after_comma.json");
    ASSERT_EQ(error, ParseError::IMPROPER_TOKEN_AFTER_COMMA);
}

