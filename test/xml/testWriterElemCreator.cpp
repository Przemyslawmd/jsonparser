
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
        std::unique_ptr<std::vector<Elem>> testWriterElemCreator(const std::string& file)
        {
            const auto root = createObjects(TEST_DATA_XML, file);
            ElemWriterCreator elemWriter(*keyMapper);
            auto elems = elemWriter.createElems(*root);
            return elems;
        }

        void testPerformance(const std::string& file)
        {
            const auto root = createObjects(TEST_DATA_XML, file);
            ElemWriterCreator elemWriter(*keyMapper);

            const auto begin = std::chrono::high_resolution_clock::now();
            for (unsigned int i = 0; i < 100; i++) {
                elemWriter.createElems(*root);
            }
            const auto end = std::chrono::high_resolution_clock::now();
            showDuration(begin, end);
        }
    };
}


static void typeAndName(const Elem& elem, const ElemType type, const std::string& name)
{
    ASSERT_EQ(elem.type, type);
    ASSERT_EQ(elem.name, name);
}


template <typename T>
static void typeAndValue(const Elem& elem, const ElemType type, const T& value)
{
    ASSERT_EQ(elem.type, type);
    ASSERT_EQ(std::get<T>(elem.value), value);
}


using enum ElemType;

TEST_F(TestWriterElemCreator, No_Declaration_1)
{
    const auto elems = testWriterElemCreator("test_no_declaration_1.xml");
    ASSERT_EQ(elems->size(), 5);

    typeAndName(elems->at(0), TAG_OPEN, "person");
    typeAndName(elems->at(1), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(2), CONTENT, "Jan");
    typeAndName(elems->at(3), TAG_CLOSE, "name");
    typeAndName(elems->at(4), TAG_CLOSE, "person");
}


TEST_F(TestWriterElemCreator, Attr_1)
{
    const auto elems = testWriterElemCreator("test_3_attr_1.xml");
    ASSERT_EQ(elems->size(), 5);

    typeAndName(elems->at(0), TAG_OPEN, "person");
    typeAndName(elems->at(1), TAG_OPEN, "name");
    ASSERT_EQ(elems->at(1).attr.size(), 1);
    ASSERT_EQ(elems->at(1).attr.at("city"), "Paris");
    typeAndValue<std::string>(elems->at(2), CONTENT, "John");
    typeAndName(elems->at(3), TAG_CLOSE, "name");
    typeAndName(elems->at(4), TAG_CLOSE, "person");
}


TEST_F(TestWriterElemCreator, Attr_2)
{
    const auto elems = testWriterElemCreator("test_3_attr_2.xml");
    ASSERT_EQ(elems->size(), 5);

    typeAndName(elems->at(0), TAG_OPEN, "person");
    ASSERT_EQ(elems->at(0).attr.size(), 1);
    ASSERT_EQ(elems->at(0).attr.at("state"), "Spain");
    typeAndName(elems->at(1), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(2), CONTENT, "John");
    typeAndName(elems->at(3), TAG_CLOSE, "name");
    typeAndName(elems->at(4), TAG_CLOSE, "person");

}


TEST_F(TestWriterElemCreator, Number_Content)
{
    const auto elems = testWriterElemCreator("test_content_number_value.xml");
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


TEST_F(TestWriterElemCreator, Array_1)
{
    const auto elems = testWriterElemCreator("test_array_1.xml");
    ASSERT_EQ(elems->size(), 8);

    typeAndName(elems->at(0), TAG_OPEN, "a");
    typeAndName(elems->at(1), TAG_ARRAY_BEGIN, "b");
    typeAndValue<std::string>(elems->at(2), CONTENT, "B");
    typeAndName(elems->at(3), TAG_ARRAY_CLOSE, "b");
    typeAndName(elems->at(4), TAG_ARRAY_OPEN, "b");
    typeAndValue<std::string>(elems->at(5), CONTENT, "C");
    typeAndName(elems->at(6), TAG_ARRAY_END, "b");
    typeAndName(elems->at(7), TAG_CLOSE, "a");
}


TEST_F(TestWriterElemCreator, Array_2)
{
    const auto elems = testWriterElemCreator("test_array_2.xml");
    ASSERT_EQ(elems->size(), 12);

    typeAndName(elems->at(0), TAG_OPEN, "root");
    typeAndName(elems->at(1), TAG_ARRAY_BEGIN, "employees");
    typeAndName(elems->at(2), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(3), CONTENT, "Agata");
    typeAndName(elems->at(4), TAG_CLOSE, "name");
    typeAndName(elems->at(5), TAG_ARRAY_CLOSE, "employees");
    typeAndName(elems->at(6), TAG_ARRAY_OPEN, "employees");
    typeAndName(elems->at(7), TAG_OPEN, "name");
    typeAndValue<std::string>(elems->at(8), CONTENT, "Anna");
    typeAndName(elems->at(9), TAG_CLOSE, "name");
    typeAndName(elems->at(10), TAG_ARRAY_END, "employees");
    typeAndName(elems->at(11), TAG_CLOSE, "root");
}


TEST_F(TestWriterElemCreator, Bigger)
{
    const auto elems = testWriterElemCreator("bigger.xml");
    ASSERT_EQ(elems->size(), 56);
}


TEST_F(TestWriterElemCreator, Performance)
{
    testPerformance("bigger.xml");
}

