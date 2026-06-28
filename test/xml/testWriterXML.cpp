
#include <chrono>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "node.h"
#include "writer/xml/writer.h"
#include "writer/xml/elemWriterCreator.h"

#include "testBaseXML.h"
#include "config.h"
#include "utilsTest.h"


using namespace xml;

class TestWriterXML : public TestBaseXML
{
protected:
    void testJsonString(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = createObjects(TEST_DATA_XML, file, *keyMapper);
        ElemWriterCreator elemWtiter(*keyMapper);
        auto elems = elemWtiter.createElems(*root);

        auto begin = std::chrono::high_resolution_clock::now();
        Writer writer(*keyMapper);
        std::string json = writer.createXmlString(elems);

        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
        std::string jsonExpected = getJsonFromFile(TEST_DATA_XML, file);
        ASSERT_EQ(json, jsonExpected);
    }
};


TEST_F(TestWriterXML, Test_File_No_Declaration_1)
{
    testJsonString("test_no_declaration_1.xml");
}

TEST_F(TestWriterXML, Test_File_No_Declaration_2)
{
    testJsonString("test_no_declaration_two_values.xml");
}

TEST_F(TestWriterXML, Test_File_No_Declaration_4)
{
    testJsonString("test_4_no_declaration.xml");
}

TEST_F(TestWriterXML, Test_File_3_Attr_1)
{
    testJsonString("test_3_attr_1.xml");
}

TEST_F(TestWriterXML, Test_File_3_Attr_2)
{
    testJsonString("test_3_attr_2.xml");
}


