
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>


using namespace xml;
using enum ElemType;

namespace
{
    class TestArrayChecker : public BaseTestXML {};
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


TEST_F(TestArrayChecker, Test_Array_1)
{
    const auto elems = createElements(TEST_DATA_XML, "test_array_1.xml");
    checkArrays(*elems);
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


TEST_F(TestArrayChecker, Array_1_Declaration)
{
    const auto elems = createElements(TEST_DATA_XML, "test_array_declaration.xml");
    checkArrays(*elems);
    ASSERT_EQ(elems->size(), 9);

    ASSERT_EQ(elems->front().type, DECLARATION);

    typeAndName(elems->at(1), TAG_OPEN, "a");
    typeAndName(elems->at(2), TAG_ARRAY_BEGIN, "b");
    typeAndValue<std::string>(elems->at(3), CONTENT, "B");
    typeAndName(elems->at(4), TAG_ARRAY_CLOSE, "b");
    typeAndName(elems->at(5), TAG_ARRAY_OPEN, "b");
    typeAndValue<std::string>(elems->at(6), CONTENT, "C");
    typeAndName(elems->at(7), TAG_ARRAY_END, "b");
    typeAndName(elems->at(8), TAG_CLOSE, "a");
}


TEST_F(TestArrayChecker, Test_Array_2)
{
    const auto elems = createElements(TEST_DATA_XML, "test_array_2.xml");
    checkArrays(*elems);
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

