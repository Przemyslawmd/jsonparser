
#include "testBaseXML.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>


using namespace xml;

namespace
{
    class TestParserTokensErrorXML : public BaseTestXML {};
}


TEST_F(TestParserTokensErrorXML, File_Angle_Open)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "angleOpen.xml");
    ASSERT_EQ(createElements(TEST_DATA_IMPROPER_XML, "angleOpen.xml"), nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_OPEN_ANGLE);
}


TEST_F(TestParserTokensErrorXML, File_Begin)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "begin.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_INVALID_BEGIN);
}


TEST_F(TestParserTokensErrorXML, File_End)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "end.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_INVALID_END);
}


TEST_F(TestParserTokensErrorXML, File_Slash_1)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "slashImproper_1.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_SLASH);
}


TEST_F(TestParserTokensErrorXML, File_Slash_2)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "slashImproper_2.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_SLASH);
}


TEST_F(TestParserTokensErrorXML, File_Angle_Close)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "angleClose.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_CLOSE_ANGLE);
}


TEST_F(TestParserTokensErrorXML, File_Declaration_Sequence)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_sequence.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, File_Declaration_Key)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_key.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, File_Declaration_No_Equal)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_no_equal.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, File_Declaration_Version)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_version.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, File_Declaration_Standalone)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_standalone.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DECLARATION);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_1)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_1.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_2)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_2.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_3)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_3.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}


TEST_F(TestParserTokensErrorXML, File_Attrs_4)
{
    const auto elems = createElements(TEST_DATA_IMPROPER_XML, "attrs_4.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_DATA_STR_QUOTA);
}

