
#include "../src/Parser.h"
#include "../src/ParserKey.h"
#include "../src/Preparser.h"
#include "../src/NodeValue.h"
#include "../src/Writer.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>


std::unique_ptr<ObjectNode> parseJSON_(const std::string& jsonFile)
{
    std::string filePath = std::string(TEST_DATA) + jsonFile;
    std::ifstream jsonStream(filePath);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>();
    return parser->parseTokens(*tokens);
}


TEST(WriterTest, Test_File_1)
{
    auto root = parseJSON_("test_1.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::cout << json << std::endl;
}