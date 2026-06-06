#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "reader/xml/preparserXML.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/elem.h"

#include "error.h"
#include "log/ErrorStorage.h"

#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestParserTokensXML : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Elem>> createElements(const std::string& path, const std::string& file)
    {
        ErrorStorage::clear();
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<PreparserXML>();
        auto tokens = preparser->parseXML(xmlString);
        auto parser = std::make_unique<ParserTokens>();
        return parser->parseTokens(std::move(tokens));
    }
};


TEST_F(TestParserTokensXML, Test_File_2)
{
    auto elems = createElements(TEST_DATA_XML, "test_2.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 4);

    ASSERT_EQ(elems->at(0).type, ElemType::DECLARATION);
    ASSERT_EQ(elems->at(0).name, "xml");
    ASSERT_EQ(elems->at(0).attr.size(), 6);
    ASSERT_EQ(std::get<std::string>(elems->at(0).attr[0].data), "version");
    ASSERT_EQ(elems->at(0).attr[1].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(0).attr[2].data), "1.0");
    ASSERT_EQ(std::get<std::string>(elems->at(0).attr[3].data), "encoding");
    ASSERT_EQ(elems->at(0).attr[4].type, TokenTypeXML::EQUAL);
    ASSERT_EQ(std::get<std::string>(elems->at(0).attr[5].data), "UTF-8");

    ASSERT_EQ(elems->at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "person");
    ASSERT_TRUE(elems->at(1).attr.empty());

    ASSERT_EQ(elems->at(2).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(2).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(2).attr[0].data), "John");

    ASSERT_EQ(elems->at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "person");
    ASSERT_TRUE(elems->at(3).attr.empty());
}


TEST_F(TestParserTokensXML, Test_File_No_Declaration_1)
{
    auto elems = createElements(TEST_DATA_XML, "test_no_declaration_1.xml");

    ASSERT_EQ(elems->size(), 5);

    ASSERT_EQ(elems->at(0).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "person");
    ASSERT_TRUE(elems->at(0).attr.empty());

    ASSERT_EQ(elems->at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "name");
    ASSERT_TRUE(elems->at(1).attr.empty());

    ASSERT_EQ(elems->at(2).type, ElemType::CONTENT);
    ASSERT_EQ(elems->at(2).name, std::nullopt);
    ASSERT_EQ(std::get<std::string>(elems->at(2).attr[0].data), "Jan");

    ASSERT_EQ(elems->at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "name");
    ASSERT_TRUE(elems->at(3).attr.empty());

    ASSERT_EQ(elems->at(4).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(4).name, "person");
    ASSERT_TRUE(elems->at(4).attr.empty());
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

