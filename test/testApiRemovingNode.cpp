
#include <iostream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"

#include "baseTestApi.h"


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::microseconds;


class ApiRemovingNode : public ApiTest {};


TEST_F(ApiRemovingNode, RemoveSimpleNodeFromObject)
{
    auto api = prepareApi("test_2.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);

    result = api->removeNodeFromObject({ "shipTo" }, "address");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemovingNode, RemoveObjectFromObject)
{
    auto api = prepareApi("test_3.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectFromObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemovingNode, RemoveObjectAndAddForTheSameKey)
{
    auto api = prepareApi("test_3.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);

    result = api->addNodeIntoObject({}, "person", Node{ .value = "newPerson" });
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectAndAddForTheSameKey_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemovingNode, RemoveArrayFromObject)
{
    auto api = prepareApi("test_7.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "employees", size_t(1) }, "data");
    const auto end = high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeArrayFromObject_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemovingNode, RemoveSimpleNodeFromArray)
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


TEST_F(ApiRemovingNode, RemoveObjectFromArray)
{
    auto api = prepareApi("test_6.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromArray({ "employees" }, size_t(0));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeObjectFromArray_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemovingNode, RemoveArrayFromArray)
{
    auto api = prepareApi("test_7.json");

    const auto begin = high_resolution_clock::now();
    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data" }, size_t(1));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    const auto elapsed = duration_cast<microseconds>(end - begin);
    std::cout << PREFIX << elapsed.count() << std::endl;
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "removeArrayFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

