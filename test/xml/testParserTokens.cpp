
#include "testBaseXML.h"

#include <memory>
#include <vector>

#include <gtest/gtest.h>


using namespace xml;
using enum ElemType;

namespace
{
    class TestParserTokensXML : public BaseTestXML
    {
    protected:
        std::unique_ptr<std::vector<Elem>> testParserTokens(const std::string& path, const std::string& file)
        {
            auto tokens = createTokens(path, file);;
            const auto parser = std::make_unique<ParserTokens>();
            auto elems = parser->parseTokens(std::move(tokens));
            return elems;
        }

        void testPerformance(const std::string& path, const std::string& file)
        {
            std::vector<std::unique_ptr<std::vector<Token>>> testTokens(100);
            for (unsigned int i = 0; i < 100; i++) {
                testTokens[i] = createTokens(path, file);
            }
            const auto parser = std::make_unique<ParserTokens>();

            const auto begin = std::chrono::high_resolution_clock::now();
            for (unsigned int i = 0; i < 100; i++) {
                parser->parseTokens(std::move(testTokens[i]));
            }
            const auto end = std::chrono::high_resolution_clock::now();
            showDuration(begin, end);
        }
    };
}


TEST_F(TestParserTokensXML, File_2)
{
    const auto elems =  testParserTokens(TEST_DATA_XML, "test_2.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 4);

    typeAndName(elems->at(0), DECLARATION, "xml");
    const auto& attrs = elems->at(0).attr;
    ASSERT_EQ(attrs.size(), 2);
    ASSERT_TRUE(attrs.contains("version"));
    ASSERT_EQ(attrs.at("version"), "1.0");
    ASSERT_TRUE(attrs.contains("encoding"));
    ASSERT_EQ(attrs.at("encoding"), "UTF-8");

    typeAndName(elems->at(1), TAG_OPEN, "person");
    typeAndValue<std::string>(elems->at(2), CONTENT, "John");
    typeAndName(elems->at(3), TAG_CLOSE, "person");
}


TEST_F(TestParserTokensXML, Number_Content)
{
    const auto elems =  testParserTokens(TEST_DATA_XML, "test_content_number_value.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 8);

    typeAndName(elems->at(0), TAG_OPEN, "person");
    typeAndName(elems->at(1), TAG_OPEN, "number");
    typeAndValue<int64_t>(elems->at(2), CONTENT, 34567);
    typeAndName(elems->at(3), TAG_CLOSE, "number");
    typeAndName(elems->at(4), TAG_OPEN, "secondNumber");
    typeAndValue<double>(elems->at(5), CONTENT, 10.002);
    typeAndName(elems->at(6), TAG_CLOSE, "secondNumber");
    typeAndName(elems->at(7), TAG_CLOSE, "person");
}


TEST_F(TestParserTokensXML, No_Declaration_1)
{
    const auto elems =  testParserTokens(TEST_DATA_XML, "test_no_declaration_1.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    typeAndName(elems->at(0), TAG_OPEN, "person");
    typeAndName(elems->at(1), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(2), CONTENT, "Jan");
    typeAndName(elems->at(3), TAG_CLOSE, "name");
    typeAndName(elems->at(4), TAG_CLOSE, "person");
}


TEST_F(TestParserTokensXML, File_5_attrs)
{
    auto elems =  testParserTokens(TEST_DATA_XML, "test_5_attrs.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 9);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, DECLARATION);
    ASSERT_EQ(elems->at(idx).name, "xml");

    const auto& attrsDec = elems->at(idx).attr;
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
    const auto& attrsCity = elems->at(idx).attr;
    ASSERT_EQ(attrsCity.size(), 1);
    ASSERT_TRUE(attrsCity.contains("state"));
    ASSERT_EQ(attrsCity.at("state"), "Italy");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");

    auto& attrsName = elems->at(idx).attr;
    ASSERT_EQ(attrsName.size(), 2);
    ASSERT_TRUE(attrsName.contains("code"));
    ASSERT_EQ(attrsName.at("code"), "ML");
    ASSERT_TRUE(attrsName.contains("post"));
    ASSERT_EQ(attrsName.at("post"), "_ML");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Milan");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "province");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Lombardy");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "province");
    ASSERT_TRUE(elems->at(idx).attr.empty());
        
    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "city");
    ASSERT_TRUE(elems->at(idx).attr.empty());
}


TEST_F(TestParserTokensXML, Content_Few_Words)
{
    auto elems =  testParserTokens(TEST_DATA_XML, "test_content_few_words.xml");

    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 5);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "John Wick Paris");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");
    ASSERT_TRUE(elems->at(idx).attr.empty());

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "person");
    ASSERT_TRUE(elems->at(idx).attr.empty());
}


TEST_F(TestParserTokensXML, Array_Before_Map)
{
    const auto elems =  testParserTokens(TEST_DATA_XML, "test_array_before_map.xml");
    ASSERT_NE(elems, nullptr);

    ASSERT_EQ(elems->at(0).type, DECLARATION);

    typeAndName(elems->at(1), TAG_OPEN, "root");
    typeAndName(elems->at(2), TAG_OPEN, "person");
    typeAndName(elems->at(3), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(4), CONTENT, "ab");
    typeAndName(elems->at(5), TAG_CLOSE, "name");
    typeAndName(elems->at(6), TAG_CLOSE, "person");
    typeAndName(elems->at(7), TAG_OPEN, "person");
    typeAndName(elems->at(8), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(9), CONTENT, "cd");
    typeAndName(elems->at(10), TAG_CLOSE, "name");
    typeAndName(elems->at(11), TAG_CLOSE, "person");
    typeAndName(elems->at(12), TAG_OPEN, "city");
    typeAndName(elems->at(13), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(14), CONTENT, "xy");
    typeAndName(elems->at(15), TAG_CLOSE, "name");
    typeAndName(elems->at(16), TAG_CLOSE, "city");
    typeAndName(elems->at(17), TAG_CLOSE, "root");
}


TEST_F(TestParserTokensXML, Bigger)
{
    const auto elems =  testParserTokens(TEST_DATA_XML, "bigger.xml");
    ASSERT_NE(elems, nullptr);
    ASSERT_EQ(elems->size(), 56);
}


TEST_F(TestParserTokensXML, Performance)
{
    testPerformance(TEST_DATA_XML, "bigger.xml");
}

