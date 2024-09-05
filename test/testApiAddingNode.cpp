
#include <iostream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;

constexpr std::string_view logPrefix = "             ###### microseconds: ";


class ApiAddingNode : public testing::Test
{
protected:

    std::unique_ptr<JsonApi> prepareApi(const std::string& file)
    {
        std::string jsonString = TestUtils::getJsonFromFile(TEST_DATA, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseJsonString(jsonString);
        EXPECT_TRUE(result);
        return api;
    }
};


TEST_F(ApiAddingNode, AddSimpleNodeToObject)
{
    auto api = prepareApi("test_4.json");
    bool result = api->addNodeIntoObject({ "person2", "address" }, "post", Node{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_simple_node_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddObjectToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("a", 123));
    newObject.emplace(std::make_pair("b", "AAA"));

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << logPrefix << elapsed.count() << std::endl;
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddNestedObjectToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", 12.45));

    std::map<std::string, Node> nestedObject;
    nestedObject.emplace(std::make_pair("qwe", "AA AA"));
    nestedObject.emplace(std::make_pair("asd", 345353));
    newObject.emplace(std::make_pair("internal", nestedObject));

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << logPrefix << elapsed.count() << std::endl;
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_nested_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddNestedObjectWithArrayToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", std::vector<Node>{ { 0 }, { 100 }, { 200 } }));

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
    std::cout << logPrefix << elapsed.count() << std::endl;
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_nested_object_with_array_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddArrayToObject)
{
    auto api = prepareApi("test_4.json");
    std::vector<Node> newArray{ { 232 }, { 234234 }, { 0 }, { 100 } };

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person2", "address" }, "dataArrayNode", { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << logPrefix << elapsed.count() << std::endl;
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_array_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddSimpleNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->addNodeIntoArray({ "employees", size_t(0), "data", size_t(0) }, Node{ .value = 4 });
    ASSERT_TRUE(result);

    result = api->insertNodeIntoArray({ "employees", size_t(1), "data", size_t(0) }, 1, Node{ .value = "c c" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addSimpleNodeToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddObjectIntoArray)
{
    auto api = prepareApi("test_7.json");
    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("aa", "bb"));
    newObjectNode.emplace(std::make_pair("cc", 12));

    bool result = api->insertNodeIntoArray({ "employees" , size_t(1), "data", size_t(0) }, 1, { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addObjectToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddingNode, AddArrayIntoArray)
{
    auto api = prepareApi("test_7.json");
    std::vector<Node> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> arr2{ { "aa" }, { "b" } };
    std::vector<Node> arr3{ { true }, { false } };
    std::vector<Node> newArrayNode{ { arr1 }, { arr2 }, { arr3 } };

    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addArrayToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

