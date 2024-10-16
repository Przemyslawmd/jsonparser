
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include "Node.h"

#include "baseTestApi.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;


class ApiChangingNode : public ApiTest {};


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoSimpleNode)
{
    auto api = prepareApi("test_3.json");
    bool result = api->changeNodeInObject({ "person" }, "country", NodeApi{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoSimpleNode_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoObject)
{
    auto api = prepareApi("test_3.json");

    std::map<std::string, NodeApi> newObjectNode;
    newObjectNode.emplace("b", true);
    newObjectNode.emplace("a", 12.45);
    newObjectNode.emplace("--", ".........");

    std::map<std::string, NodeApi> nestedObjectNode;
    nestedObjectNode.emplace("**", "ccccccccc");
    nestedObjectNode.emplace("^^", -12);
    newObjectNode.emplace("internal", nestedObjectNode);

    bool result = api->changeNodeInObject({ "person" }, "country", NodeApi{ .value = newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInObjectIntoArray)
{
    auto api = prepareApi("test_3.json");
    std::vector<NodeApi> newArrayNode{ { true }, { "abv" }, { 0 }, { 1.01 } };

    bool result = api->changeNodeInObject({ "person" }, "country", NodeApi{ .value = newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoArray_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeComplexJson)
{
    auto api = prepareApi("test_8_complex.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, NodeApi{ .value = 10 });
    ASSERT_TRUE(result);

    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", NodeApi{ .value = "Maria" });
    ASSERT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, NodeApi{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    showDuration(begin, end);

    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "changeComplexJson_8.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoSimpleNode)
{
    auto api = prepareApi("test_2.json");

    bool result = api->changeNodeInArray({ "shipTo", "cities" }, 2, NodeApi{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_simple_node_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoObject)
{
    auto api = prepareApi("test_6.json");

    std::map<std::string, NodeApi> newObject;
    newObject.emplace("aa", "bb");
    newObject.emplace("xcd", true);
    newObject.emplace("qwe", 33.45);
    newObject.emplace("qaz", -12);

    bool result = api->changeNodeInArray({ "employees" }, size_t(0), NodeApi{ .value = newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_object_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiChangingNode, ChangeNodeInArrayIntoArray)
{
    auto api = prepareApi("test_7.json");
    std::vector<NodeApi> newArray{ { 1 }, { -100 }, { 43212231231 } };

    bool result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(0) }, size_t(0), NodeApi{ .value = newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_array_7.json");
    ASSERT_EQ(json, jsonExpected);
}

/*******************************************************************/
/* ERRORS **********************************************************/

TEST_F(ApiChangingNode, ErrorImproperKeyInPath)
{
    auto api = prepareApi("test_4.json");
    NodeApi newNode{ .value = "Cracow" };

    bool result = api->changeNodeInObject({ "person2", "street" }, "address", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
}


TEST_F(ApiChangingNode, ErrorImproperKeyInNode)
{
    auto api = prepareApi("test_4.json");

    NodeApi newNode{ .value = false };
    bool result = api->changeNodeInObject({ "person2", "address" }, "bbb", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
}


TEST_F(ApiChangingNode, ErrorOutOfIndexInPath)
{
    auto api = prepareApi("test_7.json");

    NodeApi newNode{ .value = "Spain" };
    bool result = api->changeNodeInArray({ "employees", size_t(3), "data", size_t(3) }, 2, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiChangingNode, OutOfIndexInNode)
{
    auto api = prepareApi("test_7.json");

    NodeApi newNode{ .value = 23.45 };
    bool result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 4, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiChangingNode, ErrorImproperPath)
{
    auto api = prepareApi("test_4.json");

    NodeApi newNode{ .value = 12 };
    bool result = api->changeNodeInObject({ "person2", size_t(0) }, "city", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_IMPROPER_PATH);
}


TEST_F(ApiChangingNode, ErrorImproperIndicatorForNode)
{
    auto api = prepareApi("test_4.json");

    NodeApi newNode{ .value = "ABC" };
    bool result = api->changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NODE_NOT_ARRAY);
}


TEST_F(ApiChangingNode, ErrorEmptyRoot)
{
    auto api = std::make_unique<JsonApi>();

    NodeApi newNode{ .value = "ABC" };
    bool result = api->changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_EMPTY);
}

