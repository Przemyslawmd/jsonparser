
#include "../src/Parser.h"
#include "../src/Preparser.h"
#include "../src/NodeValue.h"
#include "../src/Validator.h"
#include "config.h"

#include <fstream>
#include <limits>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>


void checkStringNode(std::map<std::string, Node>* nodePointer, const std::string& key, const std::string& value)
{ 
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<std::string>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkIntNode(std::map<std::string, Node>* nodePointer, const std::string& key, int value)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<int>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkBoolNode(std::map<std::string, Node>* nodePointer, const std::string& key, bool value)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<bool>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    ASSERT_EQ(*node, value);
}


void checkDoubleNode(std::map<std::string, Node>* nodePointer, const std::string& key, double value)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<double>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    ASSERT_TRUE(*node - value <= DBL_EPSILON);
}


void checkArrayNode(std::map<std::string, Node>* nodePointer, const std::string& key)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<std::vector<Node>>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    //ASSERT_TRUE(*node - value <= DBL_EPSILON);
}


std::unique_ptr<std::map<std::string, Node>> parseJSON(const std::string& jsonFile)
{
    std::string filePath = std::string(TEST_DATA) + jsonFile;
    std::ifstream jsonStream(filePath);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    const auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto validator = std::make_unique<Validator>();
    ParseError error = validator->validate(*tokens);
    EXPECT_EQ(error, ParseError::NOT_ERROR);

    const auto parser = std::make_unique<Parser>();
    return parser->parseTokens(*tokens);
}


TEST(ParserTest, Test_File_1)
{
    auto root = parseJSON("test_1.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
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

/*
TEST(ParserTest, Test_File_3)
{
    auto root = parseJSON("test_3.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("age") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkIntNode(nodePerson, "age", 39);
    checkStringNode(nodePerson, "country", "Poland");

    ASSERT_TRUE(root->find("company") != root->end());
    ASSERT_TRUE(root->find("cities") != root->end());

    checkStringNode(root.get(), "company", "abc");

    checkArrayNode(root.get(), "cities");
    std::vector<Node>* arrayCities = std::get_if<std::vector<Node>>(&root->at("cities").value);

    auto* city_1 = std::get_if<std::string>(&arrayCities->at(0).value);
    ASSERT_TRUE(city_1 != nullptr);
    ASSERT_EQ(*city_1, std::string{ "Krakow" });
    
    auto* city_2 = std::get_if<std::string>(&arrayCities->at(1).value);
    ASSERT_TRUE(city_2 != nullptr);
    ASSERT_EQ(*city_2, std::string{ "Warszawa" });

    auto* city_3 = std::get_if<std::string>(&arrayCities->at(2).value);
    ASSERT_TRUE(city_3 != nullptr);
    ASSERT_EQ(*city_3, std::string{ "Wroclaw" });

    auto* city_4 = std::get_if<std::string>(&arrayCities->at(3).value);
    ASSERT_TRUE(city_4 != nullptr);
    ASSERT_EQ(*city_4, std::string{ "Poznan" });
}
*/

TEST(ParserTest, Test_File_4)
{
    auto root = parseJSON("test_4.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkStringNode(nodePerson, "country", "Poland");

    ASSERT_TRUE(root->find("person2") != root->end());
    auto* nodePerson2 = std::get_if<std::map<std::string, Node>>(&root->at("person2").value);

    checkStringNode(nodePerson2, "name", "John");

    ASSERT_TRUE(nodePerson2->find("address") != nodePerson2->end());
    auto* nodePerson2_Address = std::get_if<std::map<std::string, Node>>(&nodePerson2->at("address").value);

    checkStringNode(nodePerson2_Address, "city", "Cracow");
    checkStringNode(nodePerson2_Address, "street", "Kanonicza");
    checkIntNode(nodePerson2_Address, "number", 12);

    auto* nodeCompany = std::get_if<std::map<std::string, Node>>(&root->at("company").value);
    checkStringNode(nodeCompany, "name", "abc");
}


TEST(ParserTest, Test_File_5)
{
    auto root = parseJSON("test_5.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    ASSERT_TRUE(nodePerson->find("name") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("age") != nodePerson->end());
    ASSERT_TRUE(nodePerson->find("country") != nodePerson->end());

    checkStringNode(nodePerson, "name", "John");
    checkIntNode(nodePerson, "age", 39);
    checkStringNode(nodePerson, "country", "Poland");

    ASSERT_TRUE(nodePerson->find("values") != nodePerson->end());
    auto* nodeValues = std::get_if<std::map<std::string, Node>>(&nodePerson->at("values").value);
    checkDoubleNode(nodeValues, "ab", -12.67);
    checkDoubleNode(nodeValues, "cd", 43.001);

    ASSERT_TRUE(root->find("company") != root->end());
    ASSERT_TRUE(root->find("city") != root->end());

    checkStringNode(root.get(), "company", "abc");
    checkStringNode(root.get(), "city", "Cracow");
}


