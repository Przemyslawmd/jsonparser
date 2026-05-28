
#include <memory>

#include <gtest/gtest.h>

#include "reader/xml/objectCreator.h"
#include "reader/xml/preparserXML.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/elem.h"

#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestObjectCreator : public BaseTest
{
protected:
    std::unique_ptr<KeyMapper> keyMapper;

    virtual void SetUp()
    {
        keyMapper = std::make_unique<KeyMapper>();
    }

    std::unique_ptr<ObjectNode> createObjects(const std::string& path, const std::string& file)
    {
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<PreparserXML>();
        auto tokens = preparser->parseXML(xmlString);
        auto parser = std::make_unique<ParserTokens>();
        auto elems = parser->parseTokens(std::move(tokens));
        auto objCreator = std::make_unique<ObjectCreator>(*keyMapper);
        auto node = objCreator->parseElems(*elems);
        return node;
    }

    void checkKeyMapping(const std::map<uint32_t, std::string>& keyMapExpected)
    {
        for (const auto& [keyIDExpected, keyStrExpected] : keyMapExpected) {
            ASSERT_TRUE(keyMapper->getKeyStr(keyIDExpected) != std::nullopt);
            ASSERT_TRUE(keyMapper->getKeyStr(keyIDExpected).value() == keyStrExpected);
        }
    }
};


TEST_F(TestObjectCreator, test_1)
{
    auto root = createObjects(TEST_DATA_XML, "test_3.xml");
    ASSERT_NE(root, nullptr);

    std::map <uint32_t, std::string> keyMap 
    {
        { 0x00'01'00'01, "person" },
        { 0x00'02'00'01, "name" }
    };
    checkKeyMapping(keyMap);

    ASSERT_TRUE(root->find(0x00'01'00'01) != root->end());
    auto* nodeName = std::get_if<ObjectNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(nodeName != nullptr);

    auto* nodeNameContent = std::get_if<std::string>(&nodeName->at(0x00'02'00'01).value);
    ASSERT_EQ(*nodeNameContent, "John");
}


TEST_F(TestObjectCreator, test_4)
{
    auto root = createObjects(TEST_DATA_XML, "test_4.xml");
    ASSERT_NE(root, nullptr);

    std::map <uint32_t, std::string> keyMap 
    {
        { 0x00'01'00'01, "aa" },
        { 0x00'02'00'01, "bb" },
        { 0x00'03'00'01, "cc" },
        { 0x00'04'00'01, "dd" },
        { 0x00'05'00'01, "ee" },
        { 0x00'06'00'01, "ff" }

    };
    checkKeyMapping(keyMap);

    ASSERT_TRUE(root->find(0x00'01'00'01) != root->end());
    auto* nodeBB = std::get_if<ObjectNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(nodeBB != nullptr);

    auto* nodeCC = std::get_if<ObjectNode>(&nodeBB->at(0x00'02'00'01).value);
    ASSERT_TRUE(nodeCC != nullptr);

    auto* nodeDD = std::get_if<ObjectNode>(&nodeCC->at(0x00'03'00'01).value);
    ASSERT_TRUE(nodeDD != nullptr);

    auto* nodeDDContent = std::get_if<std::string>(&nodeDD->at(0x00'04'00'01).value);
    ASSERT_EQ(*nodeDDContent, "ddContent");

    auto* nodeEE = std::get_if<ObjectNode>(&nodeBB->at(0x00'02'00'02).value);
    ASSERT_TRUE(nodeEE != nullptr);

    auto* nodeFF = std::get_if<ObjectNode>(&nodeEE->at(0x00'05'00'01).value);
    ASSERT_TRUE(nodeEE != nullptr);

    auto* nodeFFContent = std::get_if<std::string>(&nodeFF->at(0x00'06'00'01).value);
    ASSERT_EQ(*nodeFFContent, "ffContent");
}


