
#include "testBaseXML.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>


class TestParserTokensXML : public TestBaseXML {};


TEST_F(TestParserTokensXML, Test_File_2)
{
    auto elems = createElements(TEST_DATA_XML, "test_2.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 4);
    
    uint index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::DECLARATION);
    ASSERT_EQ(elems->at(index).name, "xml");
    ASSERT_EQ(elems->at(index).attr.size(), 6);
    ASSERT_EQ(std::get<std::string>(elems->at(0).attr[0].data), "version");
    ASSERT_EQ(elems->at(index).attr[1].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[2].data), "1.0");
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[3].data), "encoding");
    ASSERT_EQ(elems->at(0).attr[4].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[5].data), "UTF-8");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(index).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "John");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attr.empty());
}


TEST_F(TestParserTokensXML, Test_File_No_Declaration_1)
{
    auto elems = createElements(TEST_DATA_XML, "test_no_declaration_1.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    uint index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(index).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "Jan");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attr.empty());
}


TEST_F(TestParserTokensXML, Test_File_5_attrs)
{
    auto elems = createElements(TEST_DATA_XML, "test_5_attrs.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 9);

    uint index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::DECLARATION);
    ASSERT_EQ(elems->at(index).name, "xml");
    ASSERT_EQ(elems->at(index).attr.size(), 6);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "version");
    ASSERT_EQ(elems->at(index).attr[1].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[2].data), "1.0");
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[3].data), "encoding");
    ASSERT_EQ(elems->at(index).attr[4].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[5].data), "UTF-8");
    
    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "city");    
    ASSERT_FALSE(elems->at(index).attr.empty());
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "state");
    ASSERT_EQ(elems->at(index).attr[1].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[2].data), "Italy");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "code");
    ASSERT_EQ(elems->at(index).attr[1].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[2].data), "ML");
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[3].data), "post");
    ASSERT_EQ(elems->at(index).attr[4].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[5].data), "_ML");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(index).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "Milan");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "province");
    ASSERT_TRUE(elems->at(index).attr.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(index).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(index).attr[0].data), "Lombardy");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "province");
    ASSERT_TRUE(elems->at(index).attr.empty());
        
    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "city");
    ASSERT_TRUE(elems->at(index).attr.empty());
}


TEST_F(TestParserTokensXML, Error_angle_open)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "angleOpen.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_OPEN_ANGLE);
}


TEST_F(TestParserTokensXML, Error_invalid_begin)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "begin.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_INVALID_BEGIN);
}


TEST_F(TestParserTokensXML, Error_invalid_end)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "end.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_INVALID_END);
}


TEST_F(TestParserTokensXML, Invalid_slash_1)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "slashImproper_1.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_SLASH);
}


TEST_F(TestParserTokensXML, Invalid_slash_2)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "slashImproper_2.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_SLASH);
}


TEST_F(TestParserTokensXML, AngleClose)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "angleClose.xml");
    ASSERT_EQ(elems, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PARSER_TOKENS_CLOSE_ANGLE);
}

