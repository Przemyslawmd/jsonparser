
#include <chrono>

#include <gtest/gtest.h>

#include "jsonApi.h"

#include "baseTest.h"
#include "paths.h"
#include "utilsTest.h"
#include "node.h"


using std::chrono::high_resolution_clock;
using uint = unsigned int;

namespace
{
    class ApiAddNodeJSON : public BaseTest {};
}


TEST_F(ApiAddNodeJSON, AddSimpleNodeToObject)
{
    const auto api = prepareApiWithJson("test_4.json");
    const bool result = api->addNodeIntoObject({ "person2", "address" }, "post", { "Cracow" });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "add_simple_node_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddObjectToObject)
{
    const auto api = prepareApiWithJson("test_5.json");
    std::map<std::string, NodeApi> newObject {{ "a", { 123 }}, { "b", { "AAA" }}};
    //newObject.emplace("a", 123);
    //newObject.emplace("b", "AAA");

    const bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "add_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddNestedObjectToObject)
{
    const auto api = prepareApiWithJson("test_5.json");
    std::map<std::string, NodeApi> newObject {{ "b", { true }}, { "a", { 12.45 }}};

    std::map<std::string, NodeApi> nestedObject {{ "qwe", { "AA AA" }}, { "asd", { 345353 }}};
    newObject.emplace("internal", nestedObject);
    const bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "add_nested_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddNestedObjectWithArrayToObject)
{
    const auto api = prepareApiWithJson("test_5.json");
    std::map<std::string, NodeApi> newObject {{ "b", { true }}};
    newObject.emplace("a", std::vector<NodeApi>{{ 0 }, { 100 }, { 200 }});
    const bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "add_nested_object_with_array_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddArrayToObject)
{
    const auto api = prepareApiWithJson("test_4.json");
    std::vector<NodeApi> newArray{{ 232 }, { 234234 }, { 0 }, { 100 }};
    const bool result = api->addNodeIntoObject({ "person2", "address" }, "dataArrayNode", { newArray });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "add_array_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddSimpleNodeIntoArray)
{
    const auto api = prepareApiWithJson("test_7.json");

    bool result = api->addNodeIntoArray({ "employees", static_cast<uint>(0), "data", static_cast<uint>(0) }, { 4 });
    ASSERT_TRUE(result);

    result = api->insertNodeIntoArray({ "employees", static_cast<uint>(1), "data", static_cast<uint>(0) }, 1, { "c c" });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "addSimpleNodeToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddObjectIntoArray)
{
    const auto api = prepareApiWithJson("test_7.json");
    std::map<std::string, NodeApi> newObjectNode {{ "aa", { "bb" }}, { "cc", { 12 }}};
    const bool result = api->insertNodeIntoArray({ "employees" , static_cast<uint>(1), "data", static_cast<uint>(0) },
                                                    1,
                                                    { newObjectNode });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "addObjectToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiAddNodeJSON, AddArrayIntoArray)
{
    const auto api = prepareApiWithJson("test_7.json");
    std::vector<NodeApi> arr1{{ 1 }, { 2 }, { 3 }};
    std::vector<NodeApi> arr2{{ "aa" }, { "b" }};
    std::vector<NodeApi> arr3{{ true }, { false }};
    std::vector<NodeApi> newArrayNode{{ arr1 }, { arr2 }, { arr3 }};

    const bool result = api->addNodeIntoArray({ "employees", static_cast<uint>(1), "data" }, { newArrayNode });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "addArrayToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

