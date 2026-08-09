
#include "testBaseXML.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>


using namespace xml;
using enum ElemType;

class TestParserTokensXML : public BaseTestXML {};


TEST_F(TestParserTokensXML, Test_File_2)
{
    auto elems = createElements(TEST_DATA_XML, "test_2.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 4);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, DECLARATION);
    ASSERT_EQ(elems->at(idx).name, "xml");

    const auto& attrs = elems->at(idx).attrs;
    ASSERT_EQ(attrs.size(), 2);
    ASSERT_TRUE(attrs.contains("version"));
    ASSERT_EQ(attrs.at("version"), "1.0");
    ASSERT_TRUE(attrs.contains("encoding"));
    ASSERT_EQ(attrs.at("encoding"), "UTF-8");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "John");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_Number_Content)
{
    auto elems = createElements(TEST_DATA_XML, "test_content_number_value.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 8);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "number");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<int64_t>(elems->at(idx).value), 34567);
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "number");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "secondNumber");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<double>(elems->at(idx).value), 10.002);
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "secondNumber");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_File_No_Declaration_1)
{
    auto elems = createElements(TEST_DATA_XML, "test_no_declaration_1.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Jan");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_File_5_attrs)
{
    auto elems = createElements(TEST_DATA_XML, "test_5_attrs.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 9);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, DECLARATION);
    ASSERT_EQ(elems->at(idx).name, "xml");

    const auto& attrsDec = elems->at(idx).attrs;
    ASSERT_EQ(attrsDec.size(), 3);
    ASSERT_TRUE(attrsDec.contains("version"));
    ASSERT_EQ(attrsDec.at("version"), "1.0");
    ASSERT_TRUE(attrsDec.contains("encoding"));
    ASSERT_EQ(attrsDec.at("encoding"), "UTF-8");
    ASSERT_TRUE(attrsDec.contains("standalone"));
    ASSERT_EQ(attrsDec.at("standalone"), "yes");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "city");
    const auto& attrsCity = elems->at(idx).attrs;
    ASSERT_EQ(attrsCity.size(), 1);
    ASSERT_TRUE(attrsCity.contains("state"));
    ASSERT_EQ(attrsCity.at("state"), "Italy");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");

    auto& attrsName = elems->at(idx).attrs;
    ASSERT_EQ(attrsName.size(), 2);
    ASSERT_TRUE(attrsName.contains("code"));
    ASSERT_EQ(attrsName.at("code"), "ML");
    ASSERT_TRUE(attrsName.contains("post"));
    ASSERT_EQ(attrsName.at("post"), "_ML");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Milan");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "province");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Lombardy");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "province");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
        
    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "city");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_Content_Few_Words)
{
    auto elems = createElements(TEST_DATA_XML, "test_content_few_words.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "John Wick Paris");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attrs.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attrs.empty());
}

