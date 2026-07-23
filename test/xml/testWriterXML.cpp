
#include <chrono>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "writer/xml/writer.h"
#include "writer/xml/elemWriterCreator.h"

#include "testBaseXML.h"
#include "config.h"
#include "utilsTest.h"


using namespace xml;

class TestWriterXML : public TestBaseXML
{
protected:
    void testWriter(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = createObjects(TEST_DATA_XML, file, *keyMapper);
        ElemWriterCreator elemWtiter(*keyMapper);
        auto elems = elemWtiter.createElems(*root);

        auto begin = std::chrono::high_resolution_clock::now();
        Writer writer(*keyMapper);
        std::string xml = writer.createXmlString(elems);

        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
        std::string xmlExpected = getContentFromFile(TEST_DATA_XML, file);
        ASSERT_EQ(xml, xmlExpected);
    }
};


TEST_F(TestWriterXML, Test_File_No_Declaration_1)
{
    testWriter("test_no_declaration_1.xml");
}

TEST_F(TestWriterXML, Test_File_No_Declaration_2)
{
    testWriter("test_no_declaration_two_values.xml");
}

TEST_F(TestWriterXML, Test_File_No_Declaration_4)
{
    testWriter("test_4_no_declaration.xml");
}

TEST_F(TestWriterXML, Test_File_3_Attr_1)
{
    testWriter("test_3_attr_1.xml");
}

TEST_F(TestWriterXML, Test_File_3_Attr_2)
{
    testWriter("test_3_attr_2.xml");
}

TEST_F(TestWriterXML, Test_Content_Few_Words)
{
    testWriter("test_content_few_words.xml");
}

TEST_F(TestWriterXML, Test_Content_Number)
{
    testWriter("test_content_number_value.xml");
}

TEST_F(TestWriterXML, Declaration_Two_Values__test_1)
{
    testWriter("test_1.xml");
}

TEST_F(TestWriterXML, Full_Declaration__test_5_attrs)
{
    testWriter("test_5_attrs.xml");
}

