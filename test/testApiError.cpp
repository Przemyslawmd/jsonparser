
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


class ApiTestError : public testing::Test
{
protected:

    Utils utils;
};


TEST_F(ApiTestError, ImproperKeyInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "Cracow" };
    result = api.changeNodeValue({ "person2", "street", "address" }, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getLastError(), Result::API_NOT_KEY_IN_MAP);
}


TEST_F(ApiTestError, OutOfIndexInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "Spain" };
    result = api.changeNodeValue({ "employees", size_t(3), "data", size_t(3), size_t(2) }, newNode);
    ASSERT_FALSE(result);
    Result s = api.getLastError();
    ASSERT_EQ(api.getLastError(), Result::API_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, inconsistentDataInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = 12 };
    result = api.changeNodeValue({ "person2", size_t(0), "city" }, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getLastError(), Result::API_INCONSISTENT_DATA );
}


TEST_F(ApiTestError, ImproperKeyOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = false };
    result = api.changeNodeValue({ "person2", "address", "bbb" }, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getLastError(), Result::API_NOT_KEY_IN_MAP);
}


TEST_F(ApiTestError, OutOfIndexOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = 23.45 };
    result = api.changeNodeValue({ "employees", size_t(0), "data", size_t(1), size_t(4) }, newNode);
    ASSERT_FALSE(result);
    Result s = api.getLastError();
    ASSERT_EQ(api.getLastError(), Result::API_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, inconsistentDataOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    jsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "ABC" };
    result = api.changeNodeValue({ "person2", "address", size_t(1) }, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getLastError(), Result::API_INCONSISTENT_DATA);
}