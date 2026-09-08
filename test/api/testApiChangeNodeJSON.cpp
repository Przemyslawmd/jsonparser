
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
    class ApiChangeNodeJSON : public BaseTest {};
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInObjectIntoSimpleNode)
{
    const auto api = prepareApiWithJson("test_3.json");
    const bool result = api->changeNodeInObject({ "person" }, "country", { "Spain" });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "changeNodeInObjectIntoSimpleNode_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInObjectIntoObject)
{
    auto api = prepareApiWithJson("test_3.json");

    std::map<std::string, NodeApi> newObjectNode {{ "b", { true }}, { "a", { 12.45 }},  {"--", { "........." }}};
    std::map<std::string, NodeApi> nestedObjectNode {{ "**", { "ccccccccc" }}, { "^^", { -12 }}};
    newObjectNode.emplace("internal", nestedObjectNode);

    bool result = api->changeNodeInObject({ "person" }, "country", { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "changeNodeInObjectIntoObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInObjectIntoArray)
{
    auto api = prepareApiWithJson("test_3.json");
    std::vector<NodeApi> newArrayNode{{ true }, { "abv" }, { 0 }, { 1.01 }};

    bool result = api->changeNodeInObject({ "person" }, "country", { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "changeNodeInObjectIntoArray_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeComplexJson)
{
    const auto api = prepareApiWithJson("test_8_complex.json");

    bool result = api->changeNodeInArray({ "employees", static_cast<uint>(0), "data", static_cast<uint>(1) },
                                           2,
                                           { 10 });
    ASSERT_TRUE(result);

    result = api->changeNodeInObject({ "employees", static_cast<uint>(1), "employees", static_cast<uint>(0) },
                                       "name",
                                       { "Maria" });
    ASSERT_TRUE(result);

    result = api->changeNodeInArray({ "employees",
                                       static_cast<uint>(1),
                                       "data",
                                       static_cast<uint>(2),
                                       static_cast<uint>(0),
                                       "numbers" },
                                       0,
                                       { 0.12 });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "changeComplexJson_8.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInArrayIntoSimpleNode)
{
    const auto api = prepareApiWithJson("test_2.json");

    const bool result = api->changeNodeInArray({ "shipTo", "cities" }, 2, { "Cracow" });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "change_node_in_array_into_simple_node_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInArrayIntoObject)
{
    const auto api = prepareApiWithJson("test_6.json");

    std::map<std::string, NodeApi> newObject {{ "aa", { "bb" }},
                                              { "xcd", { true }},
                                              { "qwe", { 33.45 }},
                                              { "qaz", { -12 }}};

    const bool result = api->changeNodeInArray({ "employees" }, 0, { newObject });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "change_node_in_array_into_object_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangeNodeJSON, ChangeNodeInArrayIntoArray)
{
    auto api = prepareApiWithJson("test_7.json");
    std::vector<NodeApi> newArray{{ 1 }, { -100 }, { 43212231231 }};

    const bool result = api->changeNodeInArray({ "employees", static_cast<uint>(1), "data", static_cast<uint>(0) },
                                               0,
                                               { newArray });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "change_node_in_array_into_array_7.json");
    ASSERT_EQ(json, jsonExpected);
}

