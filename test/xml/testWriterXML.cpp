
#include <chrono>

#include <gtest/gtest.h>

#include "src/writer/xml/writer.h"

#include "testBaseXML.h"
#include "utilsTest.h"

#include <iostream>
using namespace xml;

namespace
{
    class TestWriterXML : public BaseTestXML
    {
    protected:
        void testWriter(const std::string& file, unsigned int indent = 2)
        {
            auto elems = createElemsWriter(file);
            Writer writer(*keyMapper, indent);
            const std::string xmlString = writer.createXmlString(std::move(elems));
            std::cout << xmlString << std::endl;
            const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, file);
            ASSERT_EQ(xmlString, xmlExpected);
        }
    };
}


TEST_F(TestWriterXML, No_Declaration_1)
{
    testWriter("test_no_declaration_1.xml");
}

TEST_F(TestWriterXML, No_Declaration_2)
{
    testWriter("test_no_declaration_two_values.xml");
}

TEST_F(TestWriterXML, No_Declaration_4)
{
    testWriter("test_4_no_declaration.xml");
}

TEST_F(TestWriterXML, Attributes_1)
{
    testWriter("test_3_attr_1.xml");
}

TEST_F(TestWriterXML, Attributes_2)
{
    testWriter("test_3_attr_2.xml");
}

TEST_F(TestWriterXML, Content_Few_Words)
{
    testWriter("test_content_few_words.xml");
}

TEST_F(TestWriterXML, Content_Number)
{
    testWriter("test_content_number_value.xml");
}

TEST_F(TestWriterXML, Declaration_Two_Values)
{
    testWriter("test_1.xml");
}

TEST_F(TestWriterXML, Full_Declaration)
{
    testWriter("test_5_attrs.xml");
}

TEST_F(TestWriterXML, Indentation)
{
    testWriter("test_indentation_4.xml", 4);
}

TEST_F(TestWriterXML, Array_1)
{
    testWriter("test_array_1.xml", 4);
}

TEST_F(TestWriterXML, Array_2)
{
    testWriter("test_array_2.xml");
}


TEST_F(TestWriterXML, Array_Before_Map)
{
    testWriter("test_array_before_map.xml");
}
