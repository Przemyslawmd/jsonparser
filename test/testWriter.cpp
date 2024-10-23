
#include <chrono>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "node.h"
#include "reader/Parser.h"
#include "reader/ParserKey.h"
#include "reader/Preparser.h"
#include "writer/Writer.h"
#include "keyMapper.h"

#include "baseTest.h"
#include "config.h"
#include "utils.h"


std::unique_ptr<ObjectNode> writerParseJSON(const std::string& jsonFile, KeyMapper& keyMapper)
{
    std::string jsonString = getJsonFromFile(TEST_DATA, jsonFile);

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);
    tokens = createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>(keyMapper);
    return parser->parseTokens(*tokens);
}


class TestWriter : public BaseTest
{
protected:
    void testJsonString(const std::string& file)
    {
        auto keyMapper = std::make_unique<KeyMapper>();
        auto root = writerParseJSON(file, *keyMapper);

        auto begin = std::chrono::high_resolution_clock::now();
        Writer writer(*keyMapper);
        std::string json = writer.createJsonString(*root);

        const auto end = std::chrono::high_resolution_clock::now();
        showDuration(begin, end);
        std::string jsonExpected = getJsonFromFile(TEST_DATA, file);
        ASSERT_EQ(json, jsonExpected);
    }
};


TEST_F(TestWriter, Test_File_1)
{
    testJsonString("test_1.json");
}


TEST_F(TestWriter, Test_File_2)
{
    testJsonString("test_2.json");
}


TEST_F(TestWriter, Test_File_3)
{
    testJsonString("test_3.json");
}


TEST_F(TestWriter, Test_File_4)
{
    testJsonString("test_4.json");
}


TEST_F(TestWriter, Test_File_5)
{
    testJsonString("test_5.json");
}


TEST_F(TestWriter, Test_File_6)
{
    testJsonString("test_6.json");
}


TEST_F(TestWriter, Test_File_7)
{
    testJsonString("test_7.json");
}


TEST_F(TestWriter, Test_File_8)
{
    testJsonString("test_8_complex.json");
}

