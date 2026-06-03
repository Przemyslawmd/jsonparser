
#include <chrono>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "node.h"
#include "reader/xml/parserTokensXML.h"
#include "reader/xml/preparserXML.h"
#include "reader/xml/objectCreator.h"
#include "writer/xml/writer.h"

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

class TestWriterXML : public BaseTest
{
protected:
    void testJsonString(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = writerParseJSON(file, *keyMapper);

        auto begin = std::chrono::high_resolution_clock::now();
        Writer writer(*keyMapper);
        std::string json = writer.createXmlString(*root);

        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
        std::string jsonExpected = getJsonFromFile(TEST_DATA_XML, file);
        ASSERT_EQ(json, jsonExpected);
    }
};


TEST_F(TestWriterXML, Test_File_No_Declaration)
{
    testJsonString("test_no_declaration.xml");
}
