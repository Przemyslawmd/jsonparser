
#include <chrono>
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "writer/xml/writer.h"
#include "writer/xml/elemWriterCreator.h"

#include "testBaseXML.h"
#include "utilsTest.h"


using namespace xml;

namespace
{
    class TestWriterXML : public BaseTestXML
    {
    protected:
        void testWriter(const std::string& file, unsigned int indentation = 2)
        {
            const auto keyMapper = std::make_unique<KeyMapper>();
            const auto root = createObjects(TEST_DATA_XML, file, *keyMapper);
            ElemWriterCreator elemWriter(*keyMapper);
            const auto elems = elemWriter.createElems(*root);

            const auto begin = std::chrono::high_resolution_clock::now();
            Writer writer(*keyMapper, indentation);
            const std::string xml = writer.createXmlString(elems);

            const auto end = std::chrono::high_resolution_clock::now();
            showDuration(begin, end);
            const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, file);
            std::cout << xml << std::endl;
            ASSERT_EQ(xml, xmlExpected);
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


TEST_F(TestWriterXML, Array)
{
    testWriter("test_array_1.xml", 4);
}

