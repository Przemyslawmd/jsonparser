
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>


using namespace xml;
using enum ElemType;

class TestArrayChecker : public BaseTestXML {};


TEST_F(TestArrayChecker, Test_Array)
{
    auto elems = createElements(TEST_DATA_XML, "test_array.xml");
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
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_CLOSE);
    ASSERT_EQ(elems->at(idx).name, "employees");

    idx++;
    ASSERT_EQ(elems->at(idx).type, TAG_ARRAY_OPEN);
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

