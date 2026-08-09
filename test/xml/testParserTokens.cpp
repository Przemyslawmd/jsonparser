
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

    unsigned int index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::DECLARATION);
    ASSERT_EQ(elems->at(index).name, "xml");

    const auto& attrs = elems->at(index).attrs;
    ASSERT_EQ(attrs.size(), 2);
    ASSERT_TRUE(attrs.contains("version"));
    ASSERT_EQ(attrs.at("version"), "1.0");
    ASSERT_TRUE(attrs.contains("encoding"));
    ASSERT_EQ(attrs.at("encoding"), "UTF-8");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(index).value), "John");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_Number_Content)
{
    auto elems = createElements(TEST_DATA_XML, "test_content_number_value.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 8);

    unsigned int index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "number");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<int64_t>(elems->at(index).value), 34567);
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "number");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "secondNumber");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<double>(elems->at(index).value), 10.002);
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "secondNumber");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_File_No_Declaration_1)
{
    auto elems = createElements(TEST_DATA_XML, "test_no_declaration_1.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    unsigned int index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(index).value), "Jan");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_File_5_attrs)
{
    auto elems = createElements(TEST_DATA_XML, "test_5_attrs.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 9);

    unsigned int index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::DECLARATION);
    ASSERT_EQ(elems->at(index).name, "xml");

    const auto& attrsDec = elems->at(index).attrs;
    ASSERT_EQ(attrsDec.size(), 3);
    ASSERT_TRUE(attrsDec.contains("version"));
    ASSERT_EQ(attrsDec.at("version"), "1.0");
    ASSERT_TRUE(attrsDec.contains("encoding"));
    ASSERT_EQ(attrsDec.at("encoding"), "UTF-8");
    ASSERT_TRUE(attrsDec.contains("standalone"));
    ASSERT_EQ(attrsDec.at("standalone"), "yes");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "city");
    const auto& attrsCity = elems->at(index).attrs;
    ASSERT_EQ(attrsCity.size(), 1);
    ASSERT_TRUE(attrsCity.contains("state"));
    ASSERT_EQ(attrsCity.at("state"), "Italy");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "name");

    auto& attrsName = elems->at(index).attrs;
    ASSERT_EQ(attrsName.size(), 2);
    ASSERT_TRUE(attrsName.contains("code"));
    ASSERT_EQ(attrsName.at("code"), "ML");
    ASSERT_TRUE(attrsName.contains("post"));
    ASSERT_EQ(attrsName.at("post"), "_ML");

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(index).value), "Milan");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "province");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(index).value), "Lombardy");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "province");
    ASSERT_TRUE(elems->at(index).attrs.empty());
        
    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "city");
    ASSERT_TRUE(elems->at(index).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_Content_Few_Words)
{
    auto elems = createElements(TEST_DATA_XML, "test_content_few_words.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    unsigned int index = 0;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(index).value), "John Wick Paris");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "name");
    ASSERT_TRUE(elems->at(index).attrs.empty());

    index++;
    ASSERT_EQ(elems->at(index).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems->at(index).name, "person");
    ASSERT_TRUE(elems->at(index).attrs.empty());
}


TEST_F(TestParserTokensXML, Test_Array)
{
    auto elems = createElements(TEST_DATA_XML, "test_array.xml");
    ASSERT_EQ(elems->size(), 12);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "root");

    ASSERT_EQ(elems->at(1).type, TAG_ARRAY_OPEN);
    ASSERT_EQ(elems->at(1).name, "employees");

    ASSERT_EQ(elems->at(2).type, TAG_OPEN);
    ASSERT_EQ(elems->at(2).name, "name");

    ASSERT_EQ(elems->at(3).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(3).value), "Agata");

    ASSERT_EQ(elems->at(4).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(4).name, "name");

    ASSERT_EQ(elems->at(5).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(5).name, "employees");

    ASSERT_EQ(elems->at(6).type, TAG_ARRAY_OPEN);
    ASSERT_EQ(elems->at(6).name, "employees");

    ASSERT_EQ(elems->at(7).type, TAG_OPEN);
    ASSERT_EQ(elems->at(7).name, "name");

    ASSERT_EQ(elems->at(8).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(8).value), "Anna");

    ASSERT_EQ(elems->at(9).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(9).name, "name");

    ASSERT_EQ(elems->at(10).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(10).name, "employees");

    ASSERT_EQ(elems->at(11).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(11).name, "root");
}

