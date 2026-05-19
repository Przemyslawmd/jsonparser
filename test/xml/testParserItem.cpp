#include <memory>
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
    std::unique_ptr<std::vector<Item>> createTags(const std::string& path, const std::string& file)
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


TEST_F(TestParserItemXML, Test_File_2)
{
    auto tags = createTags(TEST_DATA_XML, "test_2.xml");

    ASSERT_EQ(tags->size(), 4);
    
    ASSERT_EQ(tags->at(0).type, ItemType::DECLARATION);
    ASSERT_EQ(tags->at(0).data.size(), 7);
    ASSERT_EQ(tags->at(0).data[0], "xml");
    ASSERT_EQ(tags->at(0).data[1], "version");
    ASSERT_EQ(tags->at(0).data[2], "=");
    ASSERT_EQ(tags->at(0).data[3], "1.0");
    ASSERT_EQ(tags->at(0).data[4], "encoding");
    ASSERT_EQ(tags->at(0).data[5], "=");
    ASSERT_EQ(tags->at(0).data[6], "UTF-8");

    ASSERT_EQ(tags->at(1).type, ItemType::TAG_OPEN);
    ASSERT_EQ(tags->at(1).name, "person");
    ASSERT_TRUE(tags->at(1).data.empty());

    ASSERT_EQ(tags->at(2).type, ItemType::VALUE);
    ASSERT_EQ(tags->at(2).name, std::nullopt);
    ASSERT_EQ(tags->at(2).data[0], "John");

    ASSERT_EQ(tags->at(3).type, ItemType::TAG_CLOSE);
    ASSERT_EQ(tags->at(3).name, "person");
    ASSERT_TRUE(tags->at(3).data.empty());
}

