
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>

#include "settings.h"


using namespace xml;

namespace
{
    class TestObjectCreator : public BaseTestXML
    {
    protected:
        std::unique_ptr<ObjectNode> testObjectCreator(const std::string& path, const std::string& file)
        {
            const auto elems = createElements(path, file);
            checkArrays(*elems);
            ObjectCreator objCreator(*keyMapper);

            const auto begin = std::chrono::high_resolution_clock::now();
            auto node = objCreator.parseElems(*elems);
            const auto end = std::chrono::high_resolution_clock::now();

            showDuration(begin, end);
            return node;
        }

        void checkKeyMapping(const std::map<uint32_t, std::string>& mapExpected)
        {
            for (const auto& [idExpected, strExpected] : mapExpected) {
                ASSERT_TRUE(keyMapper->getKeyStr(idExpected).has_value());
                ASSERT_TRUE(keyMapper->getKeyStr(idExpected).value() == strExpected);
            }
        }
    };
}


TEST_F(TestObjectCreator, Test_3)
{
    auto root = testObjectCreator(TEST_DATA_XML, "test_3.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t person = 0x00'01'00'01;
    constexpr uint32_t name = 0x00'02'00'01;
    const std::map <uint32_t, std::string> keys
    {
        { person, "person" },
        { name, "name" }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(person));
    auto* nodeName = std::get_if<ObjectNode>(&root->at(person).value);
    ASSERT_TRUE(nodeName);

    auto* nameContent = std::get_if<std::string>(&nodeName->at(name).value);
    ASSERT_EQ(*nameContent, "John");
}


TEST_F(TestObjectCreator, Test_4)
{
    auto root = testObjectCreator(TEST_DATA_XML, "test_4.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t aa = 0x00'01'00'01;
    constexpr uint32_t bb = 0x00'02'00'01;
    constexpr uint32_t cc = 0x00'03'00'01;
    constexpr uint32_t dd = 0x00'04'00'01;
    constexpr uint32_t ee = 0x00'05'00'01;
    constexpr uint32_t ff = 0x00'06'00'01;
    const std::map <uint32_t, std::string> keys
    {
        { aa, "aa" },
        { bb, "bb" },
        { cc, "cc" },
        { dd, "dd" },
        { ee, "ee" },
        { ff, "ff" }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(aa));
    ASSERT_EQ(root->size(), 1);

    auto* nodeAA = std::get_if<ObjectNode>(&root->at(aa).value);
    ASSERT_TRUE(nodeAA);
    ASSERT_EQ(nodeAA->size(), 1);

    auto* nodeBB = std::get_if<ObjectNode>(&nodeAA->at(bb).value);
    ASSERT_TRUE(nodeBB);
    ASSERT_EQ(nodeBB->size(), 2);

    auto* nodeCC = std::get_if<ObjectNode>(&nodeBB->at(cc).value);
    ASSERT_TRUE(nodeCC);
    ASSERT_EQ(nodeCC->size(), 1);

    auto* nodeEE = std::get_if<ObjectNode>(&nodeBB->at(ee).value);
    ASSERT_TRUE(nodeEE);
    ASSERT_EQ(nodeEE->size(), 1);

    auto* nodeDD = std::get_if<std::string>(&nodeCC->at(dd).value);
    ASSERT_TRUE(nodeDD);
    ASSERT_EQ(*nodeDD, "ddContent");

    auto* nodeFF = std::get_if<std::string>(&nodeEE->at(ff).value);
    ASSERT_TRUE(nodeFF);
    ASSERT_EQ(*nodeFF, "ffContent");
}


TEST_F(TestObjectCreator, Test_File_3_1_Attr)
{
    auto root = testObjectCreator(TEST_DATA_XML, "test_3_attr_1.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t person = 0x00'01'00'01;
    constexpr uint32_t name = 0x00'02'00'01;
    constexpr uint32_t city = 0x00'02'00'02;
    constexpr uint32_t text = 0x00'02'00'03;
    const std::map <uint32_t, std::string> keys
    {
        { person, "person" },
        { name, "name" },
        { city, "city"  },
        { text, "__text"  }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(person));
    auto* nodePerson = std::get_if<ObjectNode>(&root->at(person).value);
    ASSERT_EQ(nodePerson->size(), 1);

    auto* nodeName = std::get_if<ObjectNode>(&nodePerson->at(name).value);
    ASSERT_EQ(nodeName->size(), 2);

    auto* nameCity = std::get_if<std::string>(&nodeName->at(city).value);
    ASSERT_EQ(*nameCity, "Paris");

    auto* nameText = std::get_if<std::string>(&nodeName->at(text).value);
    ASSERT_EQ(*nameText, "John");
}


TEST_F(TestObjectCreator, Test_File_3_1_Attr_Pretended_Key_Changed)
{
    Settings::setPretendedKey("##text");

    auto root = testObjectCreator(TEST_DATA_XML, "test_3_attr_1.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t person = 0x00'01'00'01;
    constexpr uint32_t name = 0x00'02'00'01;
    constexpr uint32_t city = 0x00'02'00'02;
    constexpr uint32_t text = 0x00'02'00'03;
    const std::map <uint32_t, std::string> keys
    {
        { person, "person" },
        { name, "name", },
        { city, "city", },
        { text, "##text" }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(person));
    auto* nodePerson = std::get_if<ObjectNode>(&root->at(person).value);
    auto* nodeName = std::get_if<ObjectNode>(&nodePerson->at(name).value);

    auto* nameText = std::get_if<std::string>(&nodeName->at(text).value);
    ASSERT_EQ(*nameText, "John");

    Settings::setPretendedKey("__text");
}


TEST_F(TestObjectCreator, Test_File_3_2_Attr)
{
    auto root = testObjectCreator(TEST_DATA_XML, "test_3_attr_2.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t person = 0x00'01'00'01;
    constexpr uint32_t name = 0x00'02'00'01;
    constexpr uint32_t state = 0x00'01'00'02;
    const std::map <uint32_t, std::string> keys
    {
        { person, "person" },
        { name, "name" },
        { state, "state" },
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(person));

    auto* nodePerson = std::get_if<ObjectNode>(&root->at(person).value);
    ASSERT_EQ(nodePerson->size(), 2);

    auto* nodeName = std::get_if<std::string>(&nodePerson->at(name).value);
    ASSERT_EQ(*nodeName, "John");

    auto* nodeState = std::get_if<std::string>(&nodePerson->at(state).value);
    ASSERT_EQ(*nodeState, "Spain");
}


TEST_F(TestObjectCreator, Test_Number_Content)
{
    auto root = testObjectCreator(TEST_DATA_XML, "test_content_number_value.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t person = 0x00'01'00'01;
    constexpr uint32_t number = 0x00'02'00'01;
    constexpr uint32_t secondNumber = 0x00'03'00'01;
    const std::map <uint32_t, std::string> keys
    {
        { person,       "person" },
        { number,       "number", },
        { secondNumber, "secondNumber", }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(person));
    auto* nodeNumber = std::get_if<ObjectNode>(&root->at(person).value);
    ASSERT_TRUE(nodeNumber);

    auto* intContent = std::get_if<int64_t>(&nodeNumber->at(number).value);
    ASSERT_EQ(*intContent, 34567);

    auto* doubleContent = std::get_if<double>(&nodeNumber->at(secondNumber).value);
    ASSERT_EQ(*doubleContent, 10.002);
}


TEST_F(TestObjectCreator, Test_Array_1)
{
    const auto root = testObjectCreator(TEST_DATA_XML, "test_array_1.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t idA = 0x00'01'00'01;
    constexpr uint32_t idB = 0x00'02'00'01;
    const std::map <uint32_t, std::string> keys
    {
        { idA, "a" },
        { idB, "b" },
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(idA));
    auto* nodeA = std::get_if<ObjectNode>(&root->at(idA).value);
    ASSERT_TRUE(nodeA);

    auto* nodeB = std::get_if<ArrayNode>(&nodeA->at(idB).value);
    ASSERT_TRUE(nodeB);
    ASSERT_EQ(nodeB->size(), 2);

    ASSERT_EQ(std::get<std::string>(nodeB->at(0).value), "B");
    ASSERT_EQ(std::get<std::string>(nodeB->at(1).value), "C");
}


TEST_F(TestObjectCreator, Test_Array_2)
{
    auto const root = testObjectCreator(TEST_DATA_XML, "test_array_2.xml");
    ASSERT_TRUE(root);

    constexpr uint32_t idRoot = 0x00'01'00'01;
    constexpr uint32_t idEmp = 0x00'02'00'01;
    constexpr uint32_t idName1 = 0x00'03'00'01;
    constexpr uint32_t idName2 = 0x00'04'00'01;
    const std::map <uint32_t, std::string> keys
    {
        { idRoot,  "root" },
        { idEmp,   "employees" },
        { idName1, "name" },
        { idName2, "name" },
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->contains(idRoot));
    const auto* nodeRoot = std::get_if<ObjectNode>(&root->at(idRoot).value);
    ASSERT_TRUE(nodeRoot);

    const auto* arrayEmployees = std::get_if<ArrayNode>(&nodeRoot->at(idEmp).value);
    ASSERT_TRUE(arrayEmployees);
    ASSERT_EQ(arrayEmployees->size(), 2);

    auto* nodeName1 = std::get_if<ObjectNode>(&arrayEmployees->at(0).value);
    ASSERT_TRUE(nodeName1);
    ASSERT_EQ(std::get<std::string>(nodeName1->at(idName1).value), "Agata");

    auto* nodeName2 = std::get_if<ObjectNode>(&arrayEmployees->at(1).value);
    ASSERT_TRUE(nodeName2);
    ASSERT_EQ(std::get<std::string>(nodeName2->at(idName2).value), "Anna");
}

