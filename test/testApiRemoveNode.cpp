
#include <chrono>

#include <gtest/gtest.h>

#include "jsonApi.h"

#include "baseTest.h"
#include "config.h"
#include "utils.h"


using std::chrono::high_resolution_clock;


class ApiRemoveNode : public BaseTest {};


TEST_F(ApiRemoveNode, RemoveSimpleNodeFromObject)
{
    auto api = prepareApi("test_2.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);

    result = api->removeNodeFromObject({ "shipTo" }, "address");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);
    showDuration(begin, end);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromObject)
{
    auto api = prepareApi("test_3.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);
    showDuration(begin, end);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectFromObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectAndAddForTheSameKey)
{
    auto api = prepareApi("test_3.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);

    result = api->addNodeIntoObject({}, "person", NodeApi{ .value = "newPerson" });
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);
    showDuration(begin, end);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectAndAddForTheSameKey_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromObject)
{
    auto api = prepareApi("test_7.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "employees", size_t(1) }, "data");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);
    showDuration(begin, end);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeArrayFromObject_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveSimpleNodeFromArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeSimpleNodeFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromArray)
{
    auto api = prepareApi("test_6.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromArray({ "employees" }, size_t(0));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectFromArray_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromArray)
{
    auto api = prepareApi("test_7.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data" }, size_t(1));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeArrayFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

