
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "writer/xml/elemWriterCreator.h"

#include "testBaseXML.h"
#include "config.h"


using namespace xml;

class TestWriterElemCreator : public TestBaseXML
{
protected:
    std::vector<ElemWriter> createElemsWriter(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = createObjects(TEST_DATA_XML, file, *keyMapper);
        auto writer = std::make_unique<ElemWriterCreator>(*keyMapper);
        return writer->createElems(*root);
    }
};


TEST_F(TestWriterElemCreator, Test_File_No_Declaration_1)
{
    auto elems = createElemsWriter("test_no_declaration_1.xml");
    ASSERT_EQ(elems.size(), 5);

    ASSERT_EQ(elems.at(0).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(0).name, "person");

    ASSERT_EQ(elems.at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(1).name, "name");

    ASSERT_EQ(elems.at(2).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems.at(2).value), "Jan");

    ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(3).name, "name");

    ASSERT_EQ(elems.at(4).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Test_File_3_Attr_1)
{
    auto elems = createElemsWriter("test_3_attr_1.xml");
    ASSERT_EQ(elems.size(), 5);

    ASSERT_EQ(elems.at(0).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(0).name, "person");

    ASSERT_EQ(elems.at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(1).name, "name");
    ASSERT_EQ(elems.at(1).attr.size(), 1);
    ASSERT_EQ(elems.at(1).attr.at("city"), "Paris");

    ASSERT_EQ(elems.at(2).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems.at(2).value), "John");

    ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(3).name, "name");

    ASSERT_EQ(elems.at(4).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Test_File_3_Attr_2)
{
    auto elems = createElemsWriter("test_3_attr_2.xml");
    ASSERT_EQ(elems.size(), 5);

    ASSERT_EQ(elems.at(0).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(0).name, "person");
    ASSERT_EQ(elems.at(0).attr.size(), 1);
    ASSERT_EQ(elems.at(0).attr.at("state"), "Spain");
        
    ASSERT_EQ(elems.at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(1).name, "name");

    ASSERT_EQ(elems.at(2).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<std::string>(elems.at(2).value), "John");

    ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(3).name, "name");

    ASSERT_EQ(elems.at(4).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Test_Number_Content)
{
    auto elems = createElemsWriter("test_content_number_value.xml");
    ASSERT_EQ(elems.size(), 8);

    ASSERT_EQ(elems.at(0).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(0).name, "person");

    ASSERT_EQ(elems.at(1).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(1).name, "number");

    ASSERT_EQ(elems.at(2).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<int64_t>(elems.at(2).value), 34567);

    ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(3).name, "number");

    ASSERT_EQ(elems.at(4).type, ElemType::TAG_OPEN);
    ASSERT_EQ(elems.at(4).name, "secondNumber");

    ASSERT_EQ(elems.at(5).type, ElemType::CONTENT);
    ASSERT_EQ(std::get<double>(elems.at(5).value), 10.002);

    ASSERT_EQ(elems.at(6).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(6).name, "secondNumber");

    ASSERT_EQ(elems.at(7).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(7).name, "person");
}

