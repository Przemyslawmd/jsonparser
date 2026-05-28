
#include <memory>

#include <gtest/gtest.h>

#include "reader/xml/objectCreator.h"
#include "reader/xml/preparserXML.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/elem.h"

#include "error.h"
#include "log/ErrorStorage.h"

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
        ErrorStorage::clear();
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


