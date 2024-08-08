
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


class TestParser : public ::testing::Test
{
protected:
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<ParserKey> keyParser;
    std::unique_ptr<Preparser> preparser;
    std::unique_ptr<Validator> validator;

    virtual void SetUp()
    {
        keyMapper = std::make_unique<KeyMapper>();
        keyParser = std::make_unique<ParserKey>();
        preparser = std::make_unique<Preparser>();
        validator = std::make_unique<Validator>();
    }

    virtual void TearDown()
    {
        keyMapper.reset();
        keyParser.reset();
        preparser.reset();
        validator.reset();
    }

    std::unique_ptr<ObjectNode> parseJSON(const std::string& jsonFile)
    {
        Utils utils;
        std::string jsonString = utils.getJsonFromFile(std::string(TEST_DATA), jsonFile);

        auto tokens = preparser->parseJSON(jsonString);
        EXPECT_TRUE(tokens != nullptr);

        validator->validate(*tokens);
        tokens = keyParser->createKeyTokens(std::move(tokens));

        const auto parser = std::make_unique<Parser>(*keyMapper.get());
        auto begin = std::chrono::high_resolution_clock::now();
        std::unique_ptr<ObjectNode> jsonObj = parser->parseTokens(*tokens);

        if (measurement) {
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
        }
        return jsonObj;
    }

    void checkKeyMapping(const std::map<uint32_t, std::string>& keyMapMock)
    {
        for (const auto& [keyIDMock, keyStrMock] : keyMapMock) {
            ASSERT_TRUE(keyMapper->getStrKey(keyIDMock) != std::nullopt);
            ASSERT_TRUE(keyMapper->getStrKey(keyIDMock).value() == keyStrMock);
        }
    }
    
    /*
    template <typename T>
    void checkNode(ObjectNode* node, const std::string& key, T expectedValue)
    {
        ASSERT_TRUE(node != nullptr);
        ASSERT_TRUE(node->find(key) != node->end());
        auto* nodeValue = std::get_if<T>(&node->at(key).value);
        ASSERT_TRUE(nodeValue != nullptr);
        ASSERT_EQ(*nodeValue, expectedValue);
    }
    */
};


template <typename T>
void checkSimpleNode(ObjectNode* objectNode, const uint32_t key, T expected)
{
    ASSERT_TRUE(objectNode->find(key) != objectNode->end());
    auto* nodeValue = std::get_if<T>(&objectNode->at(key).value);
    ASSERT_TRUE(nodeValue != nullptr);
    ASSERT_EQ(*nodeValue, expected);
}


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
*/

template <typename T>
void checkArrayValue(ArrayNode* arrayPointer, size_t index, T dataExpected)
{
    try {
        T data = std::get<T>(arrayPointer->at(index).value);
        ASSERT_EQ(data, dataExpected);
    }
    catch (const std::exception& ex) {
        ASSERT_TRUE(false);
    }
}


/*
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

    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    return jsonObj;
}
*/


TEST_F(TestParser, Test_File_1)
{
    auto root = parseJSON("test_1.json");

    std::map <uint32_t, std::string> keyMap {
        { 0x00'01'00'01, "person" },
        { 0x00'02'00'01, "name" },
        { 0x00'02'00'02, "age" },
        { 0x00'02'00'03, "country" },
        { 0x00'02'00'04, "employed" },
        { 0x00'02'00'05, "restricted" },
        { 0x00'02'00'06, "empty" }};
    checkKeyMapping(keyMap);

    auto* objectPerson = std::get_if<ObjectNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(objectPerson != nullptr);

    checkSimpleNode<std::string>(objectPerson, 0x00'02'00'01, "John");
    checkSimpleNode<int64_t>(objectPerson, 0x00'02'00'02, 39);
    checkSimpleNode<std::string>(objectPerson, 0x00'02'00'03, "Poland");
    checkSimpleNode<bool>(objectPerson, 0x00'02'00'04, true);
    checkSimpleNode<bool>(objectPerson, 0x00'02'00'05, false);
}


TEST_F(TestParser, Test_File_3)
{
    auto root = parseJSON("test_3.json");

    std::map<uint32_t, std::string> keyMap{
        { 0x00'01'00'01, "person" },
        { 0x00'02'00'01, "name" },
        { 0x00'02'00'02, "age" },
        { 0x00'02'00'03, "country" },
        { 0x00'01'00'02, "company" },
        { 0x00'01'00'03, "cities" } };
    checkKeyMapping(keyMap);

    auto* objectPerson = std::get_if<ObjectNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(objectPerson != nullptr);

    checkSimpleNode<std::string>(objectPerson, 0x00'02'00'01, "John");
    checkSimpleNode<int64_t>(objectPerson, 0x00'02'00'02, 39);
    checkSimpleNode<std::string>(objectPerson, 0x00'02'00'03, "Poland");

    checkSimpleNode<std::string>(root.get(), 0x00'01'00'02, "abc");

    ArrayNode* arrayCities = std::get_if<ArrayNode>(&root->at(0x00'01'00'03).value);
    checkArrayValue<std::string>(arrayCities, 0, std::string{ "Krakow" });
    checkArrayValue<std::string>(arrayCities, 1, std::string{ "Warszawa" });
    checkArrayValue<std::string>(arrayCities, 2, std::string{ "Wroclaw" });
    checkArrayValue<std::string>(arrayCities, 3, std::string{ "Poznan" });
}

/*
TEST_F(TestParser, Test_File_6)
{
    auto root = parseJSON("test_6.json");
    ASSERT_TRUE(root != nullptr);

    std::map<std::string, uint32_t> keyMap{
    { "employees",  0x00'01'00'01 },
    { "name",       0x00'02'00'01 },
    { "email",        0x00'02'00'02 },
    { "age",    0x00'02'00'03 },
    { "name",    0x00'01'00'02 },
    { "",     0x00'01'00'03 } };
    checkKeyMapping(keyMap);
    
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

/*
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

