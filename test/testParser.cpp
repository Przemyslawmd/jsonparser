
#include <fstream>
#include <limits>
#include <memory>
#include <vector>
#include <variant>

#include <gtest/gtest.h>

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/keyMapper.h"
#include "config.h"
#include <NodeValue.h>
#include "utils.h"
#include <bitset>

constexpr bool measurement = true;

/*/
template <typename T>
void checkNode(ObjectNode* nodePointer, const std::string& key, T expectedValue)
{
    ASSERT_TRUE(nodePointer != nullptr);
    ASSERT_TRUE(nodePointer->find(key) != nodePointer->end());
    auto* nodeValue = std::get_if<T>(&nodePointer->at(key).value);
    ASSERT_TRUE(nodeValue != nullptr);
    ASSERT_EQ(*nodeValue, expectedValue);
}
*/

/*
void checkDoubleNode(ObjectNode* nodePointer, const std::string& key, double value)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<double>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
    ASSERT_TRUE(*node - value <= DBL_EPSILON);
}


void checkArrayNode(ObjectNode* nodePointer, const std::string& key)
{
    ASSERT_TRUE(nodePointer != nullptr);
    auto* node = std::get_if<std::vector<Node>>(&nodePointer->at(key).value);
    ASSERT_TRUE(node != nullptr);
}


template <typename T>
void checkArrayNodeValue(ArrayNode* arrayPointer, size_t index, T dataExpected)
{
    try {
        T data = std::get<T>(arrayPointer->at(index).value);
        ASSERT_EQ(data, dataExpected);
    }
    catch (const std::exception& ex) {
        ASSERT_TRUE(false);
    }
}
*/


std::unique_ptr<ObjectNode> parseJSON(const std::string& jsonFile)
{
    Utils utils;
    std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA), jsonFile);

    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    EXPECT_TRUE(tokens != nullptr);

    const auto validator = std::make_unique<Validator>();
    validator->validate(*tokens);

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    auto keyMapper = std::make_unique<KeyMapper>();

    auto begin = std::chrono::high_resolution_clock::now();
    const auto parser = std::make_unique<Parser>(*keyMapper.get());
    std::unique_ptr<ObjectNode> jsonObj = parser->parseTokens(*tokens);
    
    //for (auto const& [key, val] : keyMapper.get()->keyMap) {
    //    std::cout << std::bitset<32>(key)  << " : " << val << " : " << key << std::endl;
    //}

    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    return jsonObj;
}

/*
TEST(ParserTest, Test_File_1)
{
    auto root = parseJSON("test_1.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<ObjectNode>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int64_t>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");
    checkNode<bool>(nodePerson, "employed", true);
    checkNode<bool>(nodePerson, "restricted", false);
}
*/

/*
TEST(ParserTest, Test_File_3)
{
    auto root = parseJSON("test_3.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<ObjectNode>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int64_t>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");

    checkNode<std::string>(root.get(), "company", "abc");

    ASSERT_TRUE(root->find("cities") != root->end());
    checkArrayNode(root.get(), "cities");
    std::vector<Node>* arrayCities = std::get_if<ArrayNode>(&root->at("cities").value);

    checkArrayNodeValue<std::string>(arrayCities, 0, std::string{ "Krakow" });
    checkArrayNodeValue<std::string>(arrayCities, 1, std::string{ "Warszawa" });
    checkArrayNodeValue<std::string>(arrayCities, 2, std::string{ "Wroclaw" });
    checkArrayNodeValue<std::string>(arrayCities, 3, std::string{ "Poznan" });
}
*/

TEST(ParserTest, Test_File_4)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto root = parseJSON("test_4.json");

    //ASSERT_TRUE(root->find("person") != root->end());

    //auto* nodePerson = std::get_if<ObjectNode_>(&root->at("person").value);
    //ASSERT_TRUE(nodePerson != nullptr);

    //checkNode<std::string>(nodePerson, "name", "John");
    //checkNode<std::string>(nodePerson, "country", "Poland");

    //ASSERT_TRUE(root->find("person2") != root->end());
    //auto* nodePerson2 = std::get_if<ObjectNode>(&root->at("person2").value);

    //checkNode<std::string>(nodePerson2, "name", "John");

    //ASSERT_TRUE(nodePerson2->find("address") != nodePerson2->end());
    //auto* nodePerson2_Address = std::get_if<ObjectNode>(&nodePerson2->at("address").value);

    //checkNode<std::string>(nodePerson2_Address, "city", "Cracow");
    //checkNode<std::string>(nodePerson2_Address, "street", "Kanonicza");
    //checkNode<int64_t>(nodePerson2_Address, "number", 12);

    //auto* nodeCompany = std::get_if<ObjectNode>(&root->at("company").value);
    //checkNode<std::string>(nodeCompany, "name", "abc");
}

/*
TEST(ParserTest, Test_File_5)
{
    auto root = parseJSON("test_5.json");

    ASSERT_TRUE(root->find("person") != root->end());

    auto* nodePerson = std::get_if<ObjectNode>(&root->at("person").value);
    ASSERT_TRUE(nodePerson != nullptr);

    checkNode<std::string>(nodePerson, "name", "John");
    checkNode<int64_t>(nodePerson, "age", 39);
    checkNode<std::string>(nodePerson, "country", "Poland");

    ASSERT_TRUE(nodePerson->find("values") != nodePerson->end());
    auto* nodeValues = std::get_if<ObjectNode>(&nodePerson->at("values").value);
    checkDoubleNode(nodeValues, "ab", -12.67);
    checkDoubleNode(nodeValues, "cd", 43.001);

    checkNode<std::string>(root.get(), "company", "abc");
    checkNode<std::string>(root.get(), "city", "Cracow");
}


TEST(ParserTest, Test_File_6)
{
    auto root = parseJSON("test_6.json");
    ASSERT_TRUE(root->find("employees") != root->end());

    std::vector<Node>* employees = std::get_if<ArrayNode>(&root->at("employees").value);
    ASSERT_TRUE(employees != nullptr);
    
    ASSERT_TRUE(employees->size() == 2);
    auto* person1 = std::get_if<ObjectNode>(&employees->at(0).value);
    auto* person2 = std::get_if<ObjectNode>(&employees->at(1).value);
    
    checkNode<std::string>(person1, "name", "Agata");
    checkNode<std::string>(person1, "email", "agata@gmail.com");
    checkNode<int64_t>(person1, "age", 33);

    checkNode<std::string>(person2, "name", "Anna");
    checkNode<std::string>(person2, "email", "anna@gmail.com");
    checkNode<int64_t>(person2, "age", 31);
}


TEST(ParserTest, Test_File_7)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto root = parseJSON("test_7.json");
    ASSERT_TRUE(root->find("employees") != root->end());
    std::vector<Node>* employees = std::get_if<ArrayNode>(&root->at("employees").value);
    ASSERT_TRUE(employees != nullptr);
    ASSERT_TRUE(employees->size() == 2);

    auto* Agata = std::get_if<ObjectNode>(&employees->at(0).value);
    auto* Anna = std::get_if<ObjectNode>(&employees->at(1).value);

    checkNode<std::string>(Agata, "name", "Agata");
    ASSERT_TRUE(Agata->find("data") != Agata->end());
    auto* dataAgata = std::get_if<ArrayNode>(&Agata->at("data").value);
    ASSERT_TRUE(dataAgata != nullptr);
    ASSERT_TRUE(dataAgata->size() == 2);

    auto* dataAgata_1 = std::get_if<ArrayNode>(&dataAgata->at(0).value);
    ASSERT_TRUE(dataAgata_1 != nullptr);
    ASSERT_TRUE(dataAgata_1->size() == 3);

    checkArrayNodeValue<int64_t>(dataAgata_1, 0, 1);
    checkArrayNodeValue<int64_t>(dataAgata_1, 1, 2);
    checkArrayNodeValue<int64_t>(dataAgata_1, 2, 3);

    auto* dataAgata_2 = std::get_if<ArrayNode>(&dataAgata->at(1).value);
    ASSERT_TRUE(dataAgata_2 != nullptr);
    ASSERT_TRUE(dataAgata_2->size() == 3);

    checkArrayNodeValue<int64_t>(dataAgata_2, 0, 4);
    checkArrayNodeValue<int64_t>(dataAgata_2, 1, 5);
    checkArrayNodeValue<int64_t>(dataAgata_2, 2, 6);

    checkNode<std::string>(Anna, "name", "Anna");
    ASSERT_TRUE(Anna->find("data") != Anna->end());
    auto* dataAnna = std::get_if<ArrayNode>(&Anna->at("data").value);
    ASSERT_TRUE(dataAnna != nullptr);
    ASSERT_TRUE(dataAnna->size() == 2);

    auto* dataAnna_1 = std::get_if<ArrayNode>(&dataAnna->at(0).value);
    ASSERT_TRUE(dataAnna_1 != nullptr);
    ASSERT_TRUE(dataAnna_1->size() == 2);

    checkArrayNodeValue<std::string>(dataAnna_1, 0, "a");
    checkArrayNodeValue<std::string>(dataAnna_1, 1, "b");

    auto* dataAnna_2 = std::get_if<ArrayNode>(&dataAnna->at(1).value);
    ASSERT_TRUE(dataAnna_2 != nullptr);
    ASSERT_TRUE(dataAnna_2->size() == 1);
    checkArrayNodeValue<std::string>(dataAnna_2, 0, "c d e");
}


TEST(ParserTest, Test_File_8)
{
    auto root = parseJSON("test_8_complex.json");
}
*/

