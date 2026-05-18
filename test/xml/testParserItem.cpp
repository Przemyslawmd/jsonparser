#include <memory>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

#include "reader/xml/PreparserXML.h"
#include "reader/xml/ParserItem.h"
#include "reader/xml/item.h"
#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestParserItemXML : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Item>> createItems(const std::string& path, const std::string& file)
    {
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<PreparserXML>();
        auto tokens = preparser->parseXML(xmlString);
        if (!tokens) {
            return nullptr;
        }
        auto parser = std::make_unique<ParserTokens>();
        auto tags = parser->parseTokens(std::move(tokens));
        return tags;
    }
};


TEST_F(TestParserItemXML, Test_File_1)
{
    auto items = createItems(TEST_DATA_XML, "test_2.xml");

    ASSERT_EQ(items->size(), 3);

    ASSERT_EQ(items->at(0).type, ItemType::TAG_OPEN);
    ASSERT_EQ(items->at(0).name, "person");
    ASSERT_TRUE(items->at(0).data.empty());

    ASSERT_EQ(items->at(1).type, ItemType::VALUE);
    ASSERT_EQ(items->at(1).name, std::nullopt);
    ASSERT_EQ(items->at(1).data[0], "John");

    ASSERT_EQ(items->at(2).type, ItemType::TAG_CLOSE);
    ASSERT_EQ(items->at(2).name, "person");
    ASSERT_TRUE(items->at(2).data.empty());
}

