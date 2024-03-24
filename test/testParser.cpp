
#include "../src/Parser.h"
#include "../src/Preparser.h"
#include "../src/NodeValue.h"
#include "config.h"

#include <fstream>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>


void checkStringNode(std::map<std::string, NodeValue>* nodePointer, const std::string& key, const std::string& value)
{ 
    auto* node = std::get_if<std::string>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkIntNode(std::map<std::string, NodeValue>* nodePointer, const std::string& key, int value)
{
    auto* node = std::get_if<int>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkBoolNode(std::map<std::string, NodeValue>* nodePointer, const std::string& key, bool value)
{
    auto* node = std::get_if<bool>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


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

    auto* nodePerson = std::get_if<std::map<std::string, NodeValue>>(&(root->at("person").value));
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("age") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("employed") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("restricted") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkIntNode(nodePerson, "age", 39);
    checkStringNode(nodePerson, "country", "Poland");
    checkBoolNode(nodePerson, "employed", true);
    checkBoolNode(nodePerson, "restricted", false);
}


TEST(ParserTest, SecondTest)
{
    std::string filePath = std::string(TEST_DATA) + "/test_3.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    const auto tokens = preparser->parseJSON(jsonStr);
    const auto parser = std::make_unique<Parser>();
    auto root = parser->parseInitialTokens(*(tokens.get()));

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, NodeValue>>(&(root->at("person").value));
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("age") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkIntNode(nodePerson, "age", 39);
    checkStringNode(nodePerson, "country", "Poland");

    ASSERT_TRUE(root->find("company") != root->end());
    ASSERT_TRUE(root->find("city") != root->end());

    checkStringNode(root.get(), "company", "abc");
    checkStringNode(root.get(), "city", "Cracow");
}

