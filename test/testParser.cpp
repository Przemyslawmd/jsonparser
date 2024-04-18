
#include "../src/Parser.h"
#include "../src/ParserKey.h"
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


template <class T>
void checkNode(std::map<std::string, Node>* nodePointer, const std::string& key, T expectedValue)
{
    ASSERT_TRUE(nodePointer != nullptr);
    ASSERT_TRUE(nodePointer->find(key) != nodePointer->end());
    auto* nodeValue = std::get_if<T>(&nodePointer->at(key).value);
    ASSERT_TRUE(nodeValue != nullptr);
    ASSERT_EQ(*nodeValue, expectedValue);
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
}


template<class T>
void checkArrayValue(std::vector<Node>* arrayPointer, size_t index, T dataExpected)
{
    try {
        T data = std::get<T>(arrayPointer->at(index).value);
        ASSERT_EQ(data, dataExpected);
    }
    catch (const std::exception& ex) {
        ASSERT_TRUE(false);
    }
}


std::unique_ptr<std::map<std::string, Node>> parseJSON(const std::string& jsonFile)
{
    std::string filePath = std::string(TEST_DATA) + jsonFile;
    std::ifstream jsonStream(filePath);
    std::string jsonString((std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>());

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto validator = std::make_unique<Validator>();
    ParseError error = validator->validate(*tokens);
    EXPECT_EQ(error, ParseError::NOT_ERROR);

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>();
    return parser->parseTokens(*tokens);
}


TEST(ParserTest, Test_File_1)
{
    auto root = parseJSON("test_1.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");
    checkNode<bool>(nodePerson, "employed", true);
    checkNode<bool>(nodePerson, "restricted", false);
}


TEST(ParserTest, Test_File_3)
{
    auto root = parseJSON("test_3.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");

    checkNode<std::string>(root.get(), "company", "abc");

    ASSERT_TRUE(root->find("cities") != root->end());
    checkArrayNode(root.get(), "cities");
    std::vector<Node>* arrayCities = std::get_if<std::vector<Node>>(&root->at("cities").value);

    checkArrayValue<std::string>(arrayCities, 0, std::string{ "Krakow" });
    checkArrayValue<std::string>(arrayCities, 1, std::string{ "Warszawa" });
    checkArrayValue<std::string>(arrayCities, 2, std::string{ "Wroclaw" });
    checkArrayValue<std::string>(arrayCities, 3, std::string{ "Poznan" });
}


TEST(ParserTest, Test_File_4)
{
    auto root = parseJSON("test_4.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<std::string>(nodePerson, "country", "Poland");

    ASSERT_TRUE(root->find("person2") != root->end());
    auto* nodePerson2 = std::get_if<std::map<std::string, Node>>(&root->at("person2").value);

    checkNode<std::string>(nodePerson2, "name", "John");

    ASSERT_TRUE(nodePerson2->find("address") != nodePerson2->end());
    auto* nodePerson2_Address = std::get_if<std::map<std::string, Node>>(&nodePerson2->at("address").value);

    checkNode<std::string>(nodePerson2_Address, "city", "Cracow");
    checkNode<std::string>(nodePerson2_Address, "street", "Kanonicza");
    checkNode<int>(nodePerson2_Address, "number", 12);

    auto* nodeCompany = std::get_if<std::map<std::string, Node>>(&root->at("company").value);
    checkNode<std::string>(nodeCompany, "name", "abc");
}


TEST(ParserTest, Test_File_5)
{
    auto root = parseJSON("test_5.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<std::map<std::string, Node>>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");

    ASSERT_TRUE(nodePerson->find("values") != nodePerson->end());
    auto* nodeValues = std::get_if<std::map<std::string, Node>>(&nodePerson->at("values").value);
    checkDoubleNode(nodeValues, "ab", -12.67);
    checkDoubleNode(nodeValues, "cd", 43.001);

    checkNode<std::string>(root.get(), "company", "abc");
    checkNode<std::string>(root.get(), "city", "Cracow");
}


TEST(ParserTest, Test_File_6)
{
    auto root = parseJSON("test_6.json");
    ASSERT_TRUE(root->find("employees") != root->end());

    std::vector<Node>* employees = std::get_if<std::vector<Node>>(&root->at("employees").value);
    ASSERT_TRUE(employees != nullptr);
    
    ASSERT_TRUE(employees->size() == 2);
    auto* person1 = std::get_if<std::map<std::string, Node>>(&employees->at(0).value);
    auto* person2 = std::get_if<std::map<std::string, Node>>(&employees->at(1).value);
    
    checkNode<std::string>(person1, "name", "Agata");
    checkNode<std::string>(person1, "email", "agata@gmail.com");
    checkNode<int>(person1, "age", 33);

    checkNode<std::string>(person2, "name", "Anna");
    checkNode<std::string>(person2, "email", "anna@gmail.com");
    checkNode<int>(person2, "age", 31);
}


TEST(ParserTest, Test_File_7)
{
    auto root = parseJSON("test_7.json");
    ASSERT_TRUE(root->find("employees") != root->end());
    std::vector<Node>* employees = std::get_if<std::vector<Node>>(&root->at("employees").value);
    ASSERT_TRUE(employees != nullptr);
    ASSERT_TRUE(employees->size() == 2);

    auto* Agata = std::get_if<std::map<std::string, Node>>(&employees->at(0).value);
    auto* Anna = std::get_if<std::map<std::string, Node>>(&employees->at(1).value);

    checkNode<std::string>(Agata, "name", "Agata");
    ASSERT_TRUE(Agata->find("data") != Agata->end());
    auto* dataAgata = std::get_if<std::vector<Node>>(&Agata->at("data").value);
    ASSERT_TRUE(dataAgata != nullptr);
    ASSERT_TRUE(dataAgata->size() == 2);

    auto* dataAgata_1 = std::get_if<std::vector<Node>>(&dataAgata->at(0).value);
    ASSERT_TRUE(dataAgata_1 != nullptr);
    ASSERT_TRUE(dataAgata_1->size() == 3);

    checkArrayValue<int>(dataAgata_1, 0, 1);
    checkArrayValue<int>(dataAgata_1, 1, 2);
    checkArrayValue<int>(dataAgata_1, 2, 3);

    auto* dataAgata_2 = std::get_if<std::vector<Node>>(&dataAgata->at(1).value);
    ASSERT_TRUE(dataAgata_2 != nullptr);
    ASSERT_TRUE(dataAgata_2->size() == 3);

    checkArrayValue<int>(dataAgata_2, 0, 4);
    checkArrayValue<int>(dataAgata_2, 1, 5);
    checkArrayValue<int>(dataAgata_2, 2, 6);

    checkNode<std::string>(Anna, "name", "Anna");
    ASSERT_TRUE(Anna->find("data") != Anna->end());
    auto* dataAnna = std::get_if<std::vector<Node>>(&Anna->at("data").value);
    ASSERT_TRUE(dataAnna != nullptr);
    ASSERT_TRUE(dataAnna->size() == 2);

    auto* dataAnna_1 = std::get_if<std::vector<Node>>(&dataAnna->at(0).value);
    ASSERT_TRUE(dataAnna_1 != nullptr);
    ASSERT_TRUE(dataAnna_1->size() == 2);

    checkArrayValue<std::string>(dataAnna_1, 0, "a");
    checkArrayValue<std::string>(dataAnna_1, 1, "b");

    auto* dataAnna_2 = std::get_if<std::vector<Node>>(&dataAnna->at(1).value);
    ASSERT_TRUE(dataAnna_2 != nullptr);
    ASSERT_TRUE(dataAnna_2->size() == 1);
    checkArrayValue<std::string>(dataAnna_2, 0, "c d e");
}

