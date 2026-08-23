
#include <memory>
#include <vector>

#include <gtest/gtest.h>

#include "testBaseXML.h"
#include "paths.h"


using namespace xml;

namespace
{
    class TestWriterElemCreator : public BaseTestXML
    {
    protected:
        std::unique_ptr<std::vector<ElemWriter>> testWriterElemCreator(const std::string& file)
        {
            const auto root = createObjects(TEST_DATA_XML, file);
            ElemWriterCreator elemWriter(*keyMapper);

            const auto begin = std::chrono::high_resolution_clock::now();
            auto elems = elemWriter.createElems(*root);
            const auto end = std::chrono::high_resolution_clock::now();

            showDuration(begin, end);
            return elems;
        }
    };
}

using enum ElemType;

TEST_F(TestWriterElemCreator, No_Declaration_1)
{
    const auto elems = testWriterElemCreator("test_no_declaration_1.xml");
    ASSERT_EQ(elems->size(), 5);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "person");

    ASSERT_EQ(elems->at(1).type, TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "name");

    ASSERT_EQ(elems->at(2).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(2).value), "Jan");

    ASSERT_EQ(elems->at(3).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "name");

    ASSERT_EQ(elems->at(4).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Attr_1)
{
    const auto elems = testWriterElemCreator("test_3_attr_1.xml");
    ASSERT_EQ(elems->size(), 5);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "person");

    ASSERT_EQ(elems->at(1).type, TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "name");
    ASSERT_EQ(elems->at(1).attr.size(), 1);
    ASSERT_EQ(elems->at(1).attr.at("city"), "Paris");

    ASSERT_EQ(elems->at(2).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(2).value), "John");

    ASSERT_EQ(elems->at(3).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "name");

    ASSERT_EQ(elems->at(4).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Attr_2)
{
    const auto elems = testWriterElemCreator("test_3_attr_2.xml");
    ASSERT_EQ(elems->size(), 5);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "person");
    ASSERT_EQ(elems->at(0).attr.size(), 1);
    ASSERT_EQ(elems->at(0).attr.at("state"), "Spain");
        
    ASSERT_EQ(elems->at(1).type, TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "name");

    ASSERT_EQ(elems->at(2).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(2).value), "John");

    ASSERT_EQ(elems->at(3).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "name");

    ASSERT_EQ(elems->at(4).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(4).name, "person");
}


TEST_F(TestWriterElemCreator, Number_Content)
{
    const auto elems = testWriterElemCreator("test_content_number_value.xml");
    ASSERT_EQ(elems->size(), 8);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "person");

    ASSERT_EQ(elems->at(1).type, TAG_OPEN);
    ASSERT_EQ(elems->at(1).name, "number");

    ASSERT_EQ(elems->at(2).type, CONTENT);
    ASSERT_EQ(std::get<int64_t>(elems->at(2).value), 34567);

    ASSERT_EQ(elems->at(3).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(3).name, "number");

    ASSERT_EQ(elems->at(4).type, TAG_OPEN);
    ASSERT_EQ(elems->at(4).name, "secondNumber");

    ASSERT_EQ(elems->at(5).type, CONTENT);
    ASSERT_EQ(std::get<double>(elems->at(5).value), 10.002);

    ASSERT_EQ(elems->at(6).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(6).name, "secondNumber");

    ASSERT_EQ(elems->at(7).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(7).name, "person");
}


TEST_F(TestWriterElemCreator, Array_1)
{
    const auto elems = testWriterElemCreator("test_array_1.xml");
    ASSERT_EQ(elems->size(), 8);

    ASSERT_EQ(elems->at(0).type, TAG_OPEN);
    ASSERT_EQ(elems->at(0).name, "a");

    ASSERT_EQ(elems->at(1).type, TAG_ARRAY_BEGIN);
    ASSERT_EQ(elems->at(1).name, "b");

    ASSERT_EQ(elems->at(2).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(2).value), "B");

    ASSERT_EQ(elems->at(3).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(3).name, "b");

    ASSERT_EQ(elems->at(4).type, TAG_ARRAY_OPEN);
    ASSERT_EQ(elems->at(4).name, "b");

    ASSERT_EQ(elems->at(5).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(5).value), "C");

    ASSERT_EQ(elems->at(6).type, TAG_ARRAY_END);
    ASSERT_EQ(elems->at(6).name, "b");

    ASSERT_EQ(elems->at(7).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(7).name, "a");
}

