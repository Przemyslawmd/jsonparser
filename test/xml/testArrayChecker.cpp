
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>


using namespace xml;
using enum ElemType;

namespace
{
    class TestArrayChecker : public BaseTestXML {};
}


TEST_F(TestArrayChecker, Test_Array_1)
{
    auto elems = createElements(TEST_DATA_XML, "test_array_1.xml");
    checkArrays(*elems);
    ASSERT_EQ(elems->size(), 8);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "a");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_OPEN);
    ASSERT_EQ(elems->at(idx).name, "b");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "B");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_NULL);
    ASSERT_EQ(elems->at(idx).name, "b");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_NULL);
    ASSERT_EQ(elems->at(idx).name, "b");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "C");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "b");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "a");
}


TEST_F(TestArrayChecker, Test_Array_2)
{
    auto elems = createElements(TEST_DATA_XML, "test_array_2.xml");
    checkArrays(*elems);
    ASSERT_EQ(elems->size(), 12);

    unsigned int idx = 0;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "root");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_OPEN);
    ASSERT_EQ(elems->at(idx).name, "employees");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Agata");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_NULL);
    ASSERT_EQ(elems->at(idx).name, "employees");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_NULL);
    ASSERT_EQ(elems->at(idx).name, "employees");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_OPEN);
    ASSERT_EQ(elems->at(idx).name, "name");

    idx++;
    ASSERT_EQ(elems->at(idx).type, CONTENT);
    ASSERT_EQ(std::get<std::string>(elems->at(idx).value), "Anna");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "name");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "employees");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "root");
}

