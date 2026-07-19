
#include "testBaseXML.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>


using namespace xml;

class TestParserTokensErrorXML : public TestBaseXML {};


TEST_F(TestParserTokensErrorXML, Error_File_Declaration_Version)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_version.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, Error_File_Declaration_Standalone)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_standalone.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, Error_File_Attrs_1)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_1.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_2)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_2.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_3)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_3.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_4)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_4.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}

