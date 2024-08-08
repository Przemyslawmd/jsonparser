
#include <chrono>
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
#include "../src/keyMapper.h"
#include "config.h"
#include "utils.h"


constexpr bool measurement = true;


std::unique_ptr<ObjectNode> writerParseJSON(const std::string& jsonFile, KeyMapper* keyMapper)
{
    Utils utils;
    std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA), jsonFile);

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>(*keyMapper);
    return parser->parseTokens(*tokens);
}


void testJsonString(const std::string& file)
{
    auto keyMapper = std::make_unique<KeyMapper>();
    auto root = writerParseJSON(file, keyMapper.get());

    auto begin = std::chrono::high_resolution_clock::now();
    Writer writer(*keyMapper.get());
    std::string json = writer.createJsonString(*root.get()).value();

    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }

    Utils utils;
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA), file);
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


TEST(TestWriter, Test_File_8)
{
    testJsonString("test_8_complex.json");
}

