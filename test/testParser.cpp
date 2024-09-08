
#include <fstream>
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


class TestParser : public ::testing::Test
{
protected:
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Preparser> preparser;
    std::unique_ptr<Validator> validator;

    virtual void SetUp()
    {
        keyMapper = std::make_unique<KeyMapper>();
        preparser = std::make_unique<Preparser>();
        validator = std::make_unique<Validator>();
    }

    std::unique_ptr<ObjectNode> parseJSON(const std::string& jsonFile)
    {
        std::string jsonString = getJsonFromFile(TEST_DATA, jsonFile);

        auto tokens = preparser->parseJSON(jsonString);
        EXPECT_TRUE(tokens != nullptr);

        validator->validate(*tokens);
        tokens = createKeyTokens(std::move(tokens));

        const auto parser = std::make_unique<Parser>(*keyMapper);
        auto begin = std::chrono::high_resolution_clock::now();
        std::unique_ptr<ObjectNode> jsonObj = parser->parseTokens(*tokens);

        const auto end = std::chrono::high_resolution_clock::now();
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
        return jsonObj;
    }

    void checkKeyMapping(const std::map<uint32_t, std::string>& keyMapMock)
    {
        for (const auto& [keyIDMock, keyStrMock] : keyMapMock) {
            ASSERT_TRUE(keyMapper->getKeyStr(keyIDMock) != std::nullopt);
            ASSERT_TRUE(keyMapper->getKeyStr(keyIDMock).value() == keyStrMock);
        }
    }
};


template <typename T>
void checkSimpleNode(ObjectNode* objectNode, const uint32_t key, T expected)
{
    ASSERT_TRUE(objectNode->find(key) != objectNode->end());
    auto* nodeValue = std::get_if<T>(&objectNode->at(key).value);
    ASSERT_TRUE(nodeValue != nullptr);
    ASSERT_EQ(*nodeValue, expected);
}


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


TEST_F(TestParser, Test_File_6)
{
    auto root = parseJSON("test_6.json");

    std::map<uint32_t, std::string> keyMap{
        { 0x00'01'00'01, "employees" },
        { 0x00'02'00'01, "name" },
        { 0x00'02'00'02, "email" },
        { 0x00'02'00'03, "age" },
        { 0x00'03'00'01, "name" },
        { 0x00'03'00'02, "email" },
        { 0x00'03'00'03, "age" } };
    checkKeyMapping(keyMap);

    auto* arrayEmployees = std::get_if<ArrayNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(arrayEmployees != nullptr);
    ASSERT_EQ(arrayEmployees->size(), 2);

    ObjectNode* person1 = std::get_if<ObjectNode>(&arrayEmployees->at(0).value);
    ObjectNode* person2 = std::get_if<ObjectNode>(&arrayEmployees->at(1).value);
    
    checkSimpleNode<std::string>(person1, 0x00'02'00'01, "Agata");
    checkSimpleNode<std::string>(person1, 0x00'02'00'02, "agata@gmail.com");
    checkSimpleNode<int64_t>(person1, 0x00'02'00'03, 33);

    checkSimpleNode<std::string>(person2, 0x00'03'00'01, "Anna");
    checkSimpleNode<std::string>(person2, 0x00'03'00'02, "anna@gmail.com");
    checkSimpleNode<int64_t>(person2, 0x00'03'00'03, 31);
}


TEST_F(TestParser, Test_File_7)
{
    auto root = parseJSON("test_7.json");

    std::map<uint32_t, std::string> keyMap{
        { 0x00'01'00'01, "employees" },
        { 0x00'02'00'01, "name" },
        { 0x00'02'00'02, "data" },
        { 0x00'03'00'01, "name" },
        { 0x00'03'00'02, "data" } };
    checkKeyMapping(keyMap);
    
    auto* arrayEmployees = std::get_if<ArrayNode>(&root->at(0x00'01'00'01).value);
    ASSERT_TRUE(arrayEmployees != nullptr);
    ASSERT_EQ(arrayEmployees->size(), 2);

    auto* objectAgata = std::get_if<ObjectNode>(&arrayEmployees->at(0).value);
    auto* objectAnna = std::get_if<ObjectNode>(&arrayEmployees->at(1).value);

    checkSimpleNode<std::string>(objectAgata, 0x00'02'00'01, "Agata");

    ArrayNode* dataAgata = std::get_if<ArrayNode>(&objectAgata->at(0x00'02'00'02).value);
    ASSERT_TRUE(dataAgata != nullptr);
    ASSERT_TRUE(dataAgata->size() == 2);

    auto* dataAgata_1 = std::get_if<ArrayNode>(&dataAgata->at(0).value);
    ASSERT_TRUE(dataAgata_1 != nullptr);
    ASSERT_TRUE(dataAgata_1->size() == 3);

    checkArrayValue<int64_t>(dataAgata_1, 0, 1);
    checkArrayValue<int64_t>(dataAgata_1, 1, 2);
    checkArrayValue<int64_t>(dataAgata_1, 2, 3);

    auto* dataAgata_2 = std::get_if<ArrayNode>(&dataAgata->at(1).value);
    ASSERT_TRUE(dataAgata_2 != nullptr);
    ASSERT_TRUE(dataAgata_2->size() == 3);

    checkArrayValue<int64_t>(dataAgata_2, 0, 4);
    checkArrayValue<int64_t>(dataAgata_2, 1, 5);
    checkArrayValue<int64_t>(dataAgata_2, 2, 6);

    checkSimpleNode<std::string>(objectAnna, 0x00'03'00'01, "Anna");
    ASSERT_TRUE(objectAnna->find(0x00'03'00'02) != objectAnna->end());
    auto* dataAnna = std::get_if<ArrayNode>(&objectAnna->at(0x00'03'00'02).value);
    ASSERT_TRUE(dataAnna != nullptr);
    ASSERT_TRUE(dataAnna->size() == 2);

    auto* dataAnna_1 = std::get_if<ArrayNode>(&dataAnna->at(0).value);
    ASSERT_TRUE(dataAnna_1 != nullptr);
    ASSERT_TRUE(dataAnna_1->size() == 2);

    checkArrayValue<std::string>(dataAnna_1, 0, "a");
    checkArrayValue<std::string>(dataAnna_1, 1, "b");

    auto* dataAnna_2 = std::get_if<ArrayNode>(&dataAnna->at(1).value);
    ASSERT_TRUE(dataAnna_2 != nullptr);
    ASSERT_TRUE(dataAnna_2->size() == 1);
    checkArrayValue<std::string>(dataAnna_2, 0, "c d e");
}

