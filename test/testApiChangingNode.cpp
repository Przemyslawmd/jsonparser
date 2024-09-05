
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>

#include "baseTestApi.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;


class ApiChangingNode : public ApiTest {};


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoSimpleNode)
{
    auto api = prepareApi("test_3.json");
    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoSimpleNode_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoObject)
{
    auto api = prepareApi("test_3.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("b", true));
    newObjectNode.emplace(std::make_pair("a", 12.45));
    newObjectNode.emplace(std::make_pair("--", "........."));

    std::map<std::string, Node> nestedObjectNode;
    nestedObjectNode.emplace(std::make_pair("**", "ccccccccc"));
    nestedObjectNode.emplace(std::make_pair("^^", -12));
    newObjectNode.emplace(std::make_pair("internal", nestedObjectNode));

    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoArray)
{
    auto api = prepareApi("test_3.json");
    std::vector<Node> newArrayNode{ { true }, { "abv" }, { 0 }, { 1.01 } };

    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoArray_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeComplexJson)
{
    auto api = prepareApi("test_8_complex.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);

    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{ .value = "Maria" });
    ASSERT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeComplexJson_8.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoSimpleNode)
{
    auto api = prepareApi("test_2.json");

    bool result = api->changeNodeInArray({ "shipTo", "cities" }, 2, Node{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_simple_node_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoObject)
{
    auto api = prepareApi("test_6.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("aa", "bb"));
    newObject.emplace(std::make_pair("xcd", true));
    newObject.emplace(std::make_pair("qwe", 33.45));
    newObject.emplace(std::make_pair("qaz", -12));

    bool result = api->changeNodeInArray({ "employees" }, size_t(0), Node{ .value = newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_object_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::vector<Node> newArray{ { 1 }, { -100 }, { 43212231231 } };

    bool result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(0) }, size_t(0), Node{ .value = newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_array_7.json");
    ASSERT_EQ(json, jsonExpected);
}

