
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include <NodeValue.h>
#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/writer/Writer.h"
#include "config.h"
#include "utils.h"


std::unique_ptr<ObjectNode> writerParseJSON(const std::string& jsonFile)
{
    Utils utils;
    std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA), jsonFile);

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>();
    return parser->parseTokens(*tokens);
}


void testJsonString(const std::string& file)
{
    auto root = writerParseJSON(file);
    Writer writer;
    std::string json = writer.createJsonString(root.get());

    std::string filePath = std::string(TEST_DATA) + "writer/" + file;
    std::ifstream jsonStream(filePath);
    std::string jsonExpected((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    ASSERT_EQ(json, jsonExpected);
}


TEST(TestWriter, Test_File_1)
{
    testJsonString("test_1.json");
}


TEST(TestWriter, Test_File_2)
{
    testJsonString("test_2.json");
}


TEST(TestWriter, Test_File_3)
{
    testJsonString("test_3.json");
}


TEST(TestWriter, Test_File_4)
{
    testJsonString("test_4.json");
}


TEST(TestWriter, Test_File_5)
{
    testJsonString("test_5.json");
}


TEST(TestWriter, Test_File_6)
{
    testJsonString("test_6.json");
}


TEST(TestWriter, Test_File_7)
{
    testJsonString("test_7.json");
}

