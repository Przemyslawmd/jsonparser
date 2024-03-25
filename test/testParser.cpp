
#include "../src/Parser.h"
#include "../src/Preparser.h"
#include "../src/NodeValue.h"
#include "../src/Validator.h"
#include "config.h"

#include <fstream>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>


void checkStringNode(std::map<std::string, Node>* nodePointer, const std::string& key, const std::string& value)
{ 
    auto* node = std::get_if<std::string>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkIntNode(std::map<std::string, Node>* nodePointer, const std::string& key, int value)
{
    auto* node = std::get_if<int>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkBoolNode(std::map<std::string, Node>* nodePointer, const std::string& key, bool value)
{
    auto* node = std::get_if<bool>(&(nodePointer->at(key).value));
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


std::unique_ptr<std::map<std::string, Node>> parseJSON(const std::string& jsonFile)
{
    std::string filePath = std::string(TEST_DATA) + jsonFile;
    std::ifstream jsonStream(filePath);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    const auto tokens = preparser->parseJSON(jsonString);

    const auto validator = std::make_unique<Validator>();
    ParseError error = validator->validate(*tokens);
    EXPECT_EQ(error, ParseError::NOT_ERROR);

    const auto parser = std::make_unique<Parser>();
    return parser->parseTokens(*tokens);
}


TEST(ParserTest, FirstTest)
{
    auto root = parseJSON("/test_1.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&(root->at("person").value));
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
    auto root = parseJSON("/test_3.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&(root->at("person").value));
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


TEST(ParserTest, ThirdTest)
{
    auto root = parseJSON("/test_4.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&(root->at("person").value));
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkStringNode(nodePerson, "country", "Poland");

    ASSERT_TRUE(root->find("person2") != root->end());
    auto* nodePerson2 = std::get_if<std::map<std::string, Node>>(&(root->at("person2").value));

    checkStringNode(nodePerson2, "name", "John");

    ASSERT_TRUE(nodePerson2->find("address") != nodePerson2->end());
    auto* nodePerson2_Address = std::get_if<std::map<std::string, Node>>(&(nodePerson2->at("address").value));

    checkStringNode(nodePerson2_Address, "city", "Cracow");
    checkStringNode(nodePerson2_Address, "street", "Kanonicza");
    checkIntNode(nodePerson2_Address, "number", 12);

    auto* nodeCompany = std::get_if<std::map<std::string, Node>>(&(root->at("company").value));
    checkStringNode(nodeCompany, "name", "abc");
}

