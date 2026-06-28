
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>

#include "reader/xml/validator.h"


class TestValidatorXML : public TestBaseXML {};


TEST_F(TestValidatorXML, Declaration_not_start)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_not_start.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_QUESTION);
}


TEST_F(TestValidatorXML, MismatchedTag_1)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_1.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_2)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_2.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_3)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_3.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_4)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_4.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, Attr_1)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_1.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_ATTR);
}


TEST_F(TestValidatorXML, Attr_2)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_2.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_ATTR);
}


TEST_F(TestValidatorXML, Attr_3)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_3.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_ATTR);
}


TEST_F(TestValidatorXML, Attr_4)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_4.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_ATTR);
}

