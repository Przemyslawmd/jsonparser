
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
    ASSERT_EQ(elems.at(2).name, "Jan");

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
    ASSERT_EQ(elems.at(2).name, "John");

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
    ASSERT_EQ(elems.at(2).name, "John");

    ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(3).name, "name");

    ASSERT_EQ(elems.at(4).type, ElemType::TAG_CLOSE);
    ASSERT_EQ(elems.at(4).name, "person");
}

