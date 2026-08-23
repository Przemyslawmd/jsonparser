
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>

#include "src/reader/xml/validator.h"


using namespace xml;

namespace
{
    class TestValidatorXML : public BaseTestXML {};
}

TEST_F(TestValidatorXML, Declaration_not_start)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_not_start.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_QUESTION);
}


TEST_F(TestValidatorXML, MismatchedTag_1)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_1.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_FALSE(ValidateElems(*elems));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_2)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_2.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_FALSE(ValidateElems(*elems));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_3)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_3.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_FALSE(ValidateElems(*elems));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_4)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_4.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_FALSE(ValidateElems(*elems));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}



