
#include <iostream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>

#include "baseTestApi.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;


class ApiAddingNode : public ApiTest {};


TEST_F(ApiTest, ClearApi)
{
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> arr2{ { "aa" }, { "b" } };
    std::vector<Node> arr3{ { true }, { false } };
    std::vector<Node> newArray{ { arr1 }, { arr2 }, { arr3 } };

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_7.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    std::string jsonString = getJsonFromFile(TEST_DATA, "test_8_complex.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);
    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{ .value = "Maria" });
    ASSERT_TRUE(result);
    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    json = api->parseJsonObjectToString().value();
    jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_8.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    jsonString = getJsonFromFile(TEST_DATA, "test_7.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(1), "data" }, 1);
    ASSERT_TRUE(result);

    json = api->parseJsonObjectToString().value();
    auto end = high_resolution_clock::now();
    auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_7_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObject_1)
{
    std::map<std::string, Node> internalObject;
    internalObject.insert(std::make_pair("age", 39));
    internalObject.insert(std::make_pair("country", "Poland"));
    internalObject.insert(std::make_pair("employed", true));
    internalObject.insert(std::make_pair("name", "John"));
    internalObject.insert(std::make_pair("restricted", false));
    internalObject.insert(std::make_pair("empty", nullptr));
    internalObject.insert(std::make_pair("newValue", 23.1));

    std::map<std::string, Node> root;
    root.insert(std::make_pair("person", internalObject));

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(Node{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "load_json_object_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObject_2)
{
    std::vector<Node> array_1_1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> array_1_2{ { 4 }, { 5 }, { 6 } };
    std::vector<Node> array_1{ { array_1_1 }, { array_1_2 } };

    std::map<std::string, Node> object_1;
    object_1.emplace(std::make_pair("name", "Agata"));
    object_1.emplace(std::make_pair("data", array_1));

    std::vector<Node> array_2_1{ { "a" }, { "b" } };
    std::vector<Node> array_2_2{ { "c d e" } };
    std::vector<Node> array_2{ { array_2_1 }, { array_2_2 } };

    std::map<std::string, Node> object_2;
    object_2.emplace(std::make_pair("name", "Anna"));
    object_2.emplace(std::make_pair("data", array_2));

    std::vector<Node> mainArray;
    mainArray.emplace_back(object_1);
    mainArray.emplace_back(object_2);

    std::map<std::string, Node> root;
    root.emplace("employees", mainArray);

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(Node{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "load_json_object_2.json");
    ASSERT_EQ(json, jsonExpected);
}

