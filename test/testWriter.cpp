
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


constexpr std::string_view separator = "\n\n--------------------------------------------------------------------------\n\n";


class TestWriter : public ::testing::Test {
public:

    static void SetUpTestSuite() 
    {
        std::ofstream ofs;
        ofs.open(std::string(TEST_DATA) + "output.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
};


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


TEST_F(TestWriter, Test_File_1)
{
    auto root = parseJSON_("test_1.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();

}


TEST_F(TestWriter, Test_File_2)
{
    auto root = parseJSON_("test_2.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}


TEST_F(TestWriter, Test_File_3)
{
    auto root = parseJSON_("test_3.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}


TEST_F(TestWriter, Test_File_4)
{
    auto root = parseJSON_("test_4.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}


TEST_F(TestWriter, Test_File_5)
{
    auto root = parseJSON_("test_5.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}


TEST_F(TestWriter, Test_File_6)
{
    auto root = parseJSON_("test_6.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}


TEST_F(TestWriter, Test_File_7)
{
    auto root = parseJSON_("test_7.json");
    Writer writer;
    std::string json = writer.createJsonString(root.get());
    std::ofstream out(std::string(TEST_DATA) + "output.txt", std::ios_base::app);
    out << json << separator;
    out.close();
}

