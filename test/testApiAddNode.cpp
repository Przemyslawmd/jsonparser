
#include <iostream>

#include <gtest/gtest.h>

#include "jsonApi.h"

#include "baseTest.h"
#include "config.h"
#include "utils.h"
#include "node.h"


using std::chrono::high_resolution_clock;


class ApiAddNode : public BaseTest {};


TEST_F(ApiAddNode, AddSimpleNodeToObject)
{
    auto api = prepareApi("test_4.json");
    bool result = api->addNodeIntoObject({ "person2", "address" }, "post", NodeApi{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "add_simple_node_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddObjectToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, NodeApi> newObject;
    newObject.emplace("a", 123);
    newObject.emplace("b", "AAA");

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "add_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddNestedObjectToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, NodeApi> newObject;
    newObject.emplace("b", true);
    newObject.emplace("a", 12.45);

    std::map<std::string, NodeApi> nestedObject;
    nestedObject.emplace("qwe", "AA AA");
    nestedObject.emplace("asd", 345353);
    newObject.emplace("internal", nestedObject);

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "add_nested_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddNestedObjectWithArrayToObject)
{
    auto api = prepareApi("test_5.json");
    std::map<std::string, NodeApi> newObject;
    newObject.emplace("b", true);
    newObject.emplace("a", std::vector<NodeApi>{ { 0 }, { 100 }, { 200 } });

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = std::chrono::high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "add_nested_object_with_array_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddArrayToObject)
{
    auto api = prepareApi("test_4.json");
    std::vector<NodeApi> newArray{ { 232 }, { 234234 }, { 0 }, { 100 } };

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoObject({ "person2", "address" }, "dataArrayNode", { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "add_array_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddSimpleNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->addNodeIntoArray({ "employees", size_t(0), "data", size_t(0) }, NodeApi{ .value = 4 });
    ASSERT_TRUE(result);

    result = api->insertNodeIntoArray({ "employees", size_t(1), "data", size_t(0) }, 1, NodeApi{ .value = "c c" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "addSimpleNodeToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddObjectIntoArray)
{
    auto api = prepareApi("test_7.json");
    std::map<std::string, NodeApi> newObjectNode;
    newObjectNode.emplace("aa", "bb");
    newObjectNode.emplace("cc", 12);

    bool result = api->insertNodeIntoArray({ "employees" , size_t(1), "data", size_t(0) }, 1, { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "addObjectToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNode, AddArrayIntoArray)
{
    auto api = prepareApi("test_7.json");
    std::vector<NodeApi> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<NodeApi> arr2{ { "aa" }, { "b" } };
    std::vector<NodeApi> arr3{ { true }, { false } };
    std::vector<NodeApi> newArrayNode{ { arr1 }, { arr2 }, { arr3 } };

    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "addArrayToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

