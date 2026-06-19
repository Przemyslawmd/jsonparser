
#include "testBaseXML.h"

#include <memory>

#include <gtest/gtest.h>


class TestObjectCreator : public TestBaseXML
{
protected:
    std::unique_ptr<KeyMapper> keyMapper;
    
    virtual void SetUp()
    {
        keyMapper = std::make_unique<KeyMapper>();
    }

    void checkKeyMapping(const std::map<std::string, uint32_t>& mapExpected)
    {
        for (const auto& [strExpected, idExpected] : mapExpected) {
            ASSERT_TRUE(keyMapper->getKeyStr(idExpected) != std::nullopt);
            ASSERT_TRUE(keyMapper->getKeyStr(idExpected).value() == strExpected);
        }
    }
};


TEST_F(TestObjectCreator, test_1)
{
    auto root = createObjects(TEST_DATA_XML, "test_3.xml", *keyMapper);
    ASSERT_NE(root, nullptr);

    std::map <std::string, uint32_t> keys 
    {
        { "person", 0x00'01'00'01, },
        { "name",   0x00'02'00'01, }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->find(keys["person"]) != root->end());
    auto* nodeName = std::get_if<ObjectNode>(&root->at(keys["person"]).value);
    ASSERT_TRUE(nodeName != nullptr);

    auto* nodeNameContent = std::get_if<std::string>(&nodeName->at(keys["name"]).value);
    ASSERT_EQ(*nodeNameContent, "John");
}


TEST_F(TestObjectCreator, test_4)
{
    auto root = createObjects(TEST_DATA_XML, "test_4.xml", *keyMapper);
    ASSERT_NE(root, nullptr);

    std::map <std::string, uint32_t> keys 
    {
        { "aa", 0x00'01'00'01 },
        { "bb", 0x00'02'00'01 },
        { "cc", 0x00'03'00'01 },
        { "dd", 0x00'04'00'01 },
        { "ee", 0x00'05'00'01 },
        { "ff", 0x00'06'00'01 }

    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->find(keys["aa"]) != root->end());
    ASSERT_EQ(root->size(), 1);

    auto* nodeAA = std::get_if<ObjectNode>(&root->at(keys["aa"]).value);
    ASSERT_TRUE(nodeAA != nullptr);
    ASSERT_EQ(nodeAA->size(), 1);

    auto* nodeBB = std::get_if<ObjectNode>(&nodeAA->at(keys["bb"]).value);
    ASSERT_TRUE(nodeBB != nullptr);
    ASSERT_EQ(nodeBB->size(), 2);

    auto* nodeCC = std::get_if<ObjectNode>(&nodeBB->at(keys["cc"]).value);
    ASSERT_TRUE(nodeCC != nullptr);
    ASSERT_EQ(nodeCC->size(), 1);

    auto* nodeEE = std::get_if<ObjectNode>(&nodeBB->at(keys["ee"]).value);
    ASSERT_TRUE(nodeEE != nullptr);
    ASSERT_EQ(nodeEE->size(), 1);

    auto* nodeDD = std::get_if<std::string>(&nodeCC->at(keys["dd"]).value);
    ASSERT_TRUE(nodeDD != nullptr);
    ASSERT_EQ(*nodeDD, "ddContent");

    auto* nodeFF = std::get_if<std::string>(&nodeEE->at(keys["ff"]).value);
    ASSERT_TRUE(nodeFF != nullptr);
    ASSERT_EQ(*nodeFF, "ffContent");
}


TEST_F(TestObjectCreator, Test_File_3_1_Attr)
{
    auto root = createObjects(TEST_DATA_XML, "test_3_attr_1.xml", *keyMapper);
    ASSERT_NE(root, nullptr);

    std::map <std::string, uint32_t> keys
    {
        { "person", 0x00'01'00'01, },
        { "name",   0x00'02'00'01, },
        { "city",   0x00'02'00'02  },
        { "__text", 0x00'02'00'03  }
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->find(keys["person"]) != root->end());
    auto* nodePerson = std::get_if<ObjectNode>(&root->at(keys["person"]).value);
    ASSERT_EQ(nodePerson->size(), 1);

    auto* nodeName = std::get_if<ObjectNode>(&nodePerson->at(keys["name"]).value);
    ASSERT_EQ(nodeName->size(), 2);

    auto* nodeNameCity = std::get_if<std::string>(&nodeName->at(keys["city"]).value);
    ASSERT_EQ(*nodeNameCity, "Paris");

    auto* nodeNameText = std::get_if<std::string>(&nodeName->at(keys["__text"]).value);
    ASSERT_EQ(*nodeNameText, "John");
}


TEST_F(TestObjectCreator, Test_File_3_2_Attr)
{
    auto root = createObjects(TEST_DATA_XML, "test_3_attr_2.xml", *keyMapper);
    ASSERT_NE(root, nullptr);

    std::map <std::string, uint32_t> keys
    {
        { "person", 0x00'01'00'01 },
        { "name",   0x00'02'00'01 },
        { "state",  0x00'01'00'02 },
    };
    checkKeyMapping(keys);

    ASSERT_TRUE(root->find(keys["person"]) != root->end());

    auto* nodePerson = std::get_if<ObjectNode>(&root->at(keys["person"]).value);
    ASSERT_EQ(nodePerson->size(), 2);

    auto* nodeName = std::get_if<std::string>(&nodePerson->at(keys["name"]).value);
    ASSERT_EQ(*nodeName, "John");

    auto* nodeState = std::get_if<std::string>(&nodePerson->at(keys["state"]).value);
    ASSERT_EQ(*nodeState, "Spain");
}

