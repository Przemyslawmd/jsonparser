
#include <chrono>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "keyMapper.h"
#include "node.h"
#include "reader/json/parser.h"
#include "reader/json/parserKey.h"
#include "reader/json/preparser.h"
#include "writer/json/writer.h"

#include "baseTest.h"
#include "paths.h"
#include "utilsTest.h"


using namespace json;

static std::unique_ptr<ObjectNode> writerParseJSON(const std::string& jsonFile, KeyMapper& keyMapper)
{
    std::string jsonString = getContentFromFile(TEST_DATA_JSON, jsonFile);

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);
    createKeyTokens(*tokens);
    const auto parser = std::make_unique<Parser>(keyMapper);
    return parser->parseTokens(*tokens);
}


class TestWriterJSON : public BaseTest
{
protected:
    void testJsonString(const std::string& file, unsigned int indentation = 2)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = writerParseJSON(file, *keyMapper);

        Writer writer(*keyMapper, indentation);
        std::string json = writer.createJsonString(*root);
        std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, file);
        ASSERT_EQ(json, jsonExpected);
    }
};


TEST_F(TestWriterJSON, Test_File_1)
{
    testJsonString("test_1.json");
}


TEST_F(TestWriterJSON, Test_File_2)
{
    testJsonString("test_2.json");
}


TEST_F(TestWriterJSON, Test_File_3)
{
    testJsonString("test_3.json");
}


TEST_F(TestWriterJSON, Test_File_4)
{
    testJsonString("test_4.json");
}


TEST_F(TestWriterJSON, Test_File_5)
{
    testJsonString("test_5.json");
}


TEST_F(TestWriterJSON, Test_File_6)
{
    testJsonString("test_6.json");
}


TEST_F(TestWriterJSON, Test_File_7)
{
    testJsonString("test_7.json");
}


TEST_F(TestWriterJSON, Test_File_8)
{
    testJsonString("test_8_complex.json");
}


TEST_F(TestWriterJSON, Indentation_3)
{
    testJsonString("test_indentation_3.json", 3);
}

