
#include <memory>

#include <gtest/gtest.h>

#include "reader/xml/objectCreator.h"
#include "reader/xml/preparserXML.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/elem.h"

#include "error.h"
#include "log/ErrorStorage.h"
#include "reader/xml/validator.h"

#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestObjectCreator : public BaseTest
{
protected:
    std::unique_ptr<ObjectNode> createObjects(const std::string& path, const std::string& file)
    {
        std::unique_ptr<KeyMapper> keyMapper = std::make_unique<KeyMapper>();
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
};


TEST_F(TestObjectCreator, test_1)
{
    auto node = createObjects(TEST_DATA_XML, "test_3.xml");
    ASSERT_NE(node, nullptr);

    ASSERT_TRUE(node->find(0x00'01'00'01) != node->end());
    auto* nodeValue = std::get_if<ObjectNode>(&node->at(0x00'01'00'01).value);
    ASSERT_TRUE(nodeValue != nullptr);

    auto* nodeValue_1 = std::get_if<std::string>(&nodeValue->at(0x00'02'00'01).value);
    ASSERT_EQ(*nodeValue_1, "QAZ");
}


