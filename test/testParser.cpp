
#include "../src/Parser.h"
#include "../src/Preparser.h"
#include "../src/TokenValue.h"
#include "config.h"

#include <fstream>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

TEST(ParserTest, FirstTest)
{
    std::string filePath = std::string(TEST_DATA) + "/test_1.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    const auto tokens = preparser->parseJSON(jsonStr);
    const auto parser = std::make_unique<Parser>();
    auto root = parser->parseInitialTokens(*(tokens.get()));

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, TokenValue>>(&(root->at("person").value));
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("age") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    auto* nodeName = std::get_if<std::string>(&(nodePerson->at("name").value));
    ASSERT_TRUE(nodeName != nullptr);
    ASSERT_EQ(*nodeName, "John");

    auto* nodeAge = std::get_if<int>(&(nodePerson->at("age").value));
    ASSERT_TRUE(nodeAge != nullptr);
    ASSERT_EQ(*nodeAge, 39);

    auto* nodeCountry = std::get_if<std::string>(&(nodePerson->at("country").value));
    ASSERT_TRUE(nodeCountry != nullptr);
    ASSERT_EQ(*nodeCountry, "Poland");
}

