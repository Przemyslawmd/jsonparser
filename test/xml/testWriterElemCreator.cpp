
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "node.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/preparserXML.h"
#include "reader/xml/objectCreator.h"
#include "writer/xml/elemWriterCreator.h"

#include "baseTest.h"
#include "config.h"
#include "utilsTest.h"


static std::unique_ptr<ObjectNode> writerParseJSON(const std::string& jsonFile, KeyMapper& keyMapper)
{
    std::string xmlString = getJsonFromFile(TEST_DATA_XML, jsonFile);
    auto preparser = std::make_unique<PreparserXML>();
    auto tokens = preparser->parseXML(xmlString);
    auto parser = std::make_unique<ParserTokens>();
    auto elems = parser->parseTokens(std::move(tokens));
    auto objCreator = std::make_unique<ObjectCreator>(keyMapper);
    auto node = objCreator->parseElems(*elems);
    return node;
}

using namespace xml;

class TestWriterElemCreator : public BaseTest
{
protected:
    void testJsonString(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = writerParseJSON(file, *keyMapper);      
        auto writer = std::make_unique<ElemWriterCreator>(*keyMapper);        
        auto elems = writer->createElems(*root);

        ASSERT_EQ(elems.size(), 5);

        ASSERT_EQ(elems.at(0).type, ElemType::TAG_OPEN);
        ASSERT_EQ(elems.at(0).name, "person");
        
        ASSERT_EQ(elems.at(1).type, ElemType::TAG_OPEN);
        ASSERT_EQ(elems.at(1).name, "name");

        ASSERT_EQ(elems.at(2).type, ElemType::CONTENT);
        ASSERT_EQ(elems.at(2).name, "Jan");

        ASSERT_EQ(elems.at(3).type, ElemType::TAG_CLOSE);
        ASSERT_EQ(elems.at(3).name, "name");

        ASSERT_EQ(elems.at(4).type, ElemType::TAG_CLOSE);
        ASSERT_EQ(elems.at(4).name, "person");
    }
};


TEST_F(TestWriterElemCreator, Test_File_No_Declaration_1)
{
    testJsonString("test_no_declaration_1.xml");
}


