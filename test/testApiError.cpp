
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


class ApiTestError : public testing::Test
{
protected:

    TestUtils utils;
};


TEST_F(ApiTestError, ImproperKeyInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "Cracow" };
    result = api.changeNodeInObject({ "person2", "street" }, "address", newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_NOT_KEY_IN_MAP);
}


TEST_F(ApiTestError, OutOfIndexInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "Spain" };
    result = api.changeNodeInArray({ "employees", size_t(3), "data", size_t(3) }, 2, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, inconsistentDataInnerNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = 12 };
    result = api.changeNodeInObject({ "person2", size_t(0) }, "city", newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_INCONSISTENT_DATA);
}


TEST_F(ApiTestError, ImproperKeyOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = false };
    result = api.changeNodeInObject({ "person2", "address" }, "bbb", newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_NOT_KEY_IN_MAP);
}


TEST_F(ApiTestError, OutOfIndexOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = 23.45 };
    result = api.changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 4, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, inconsistentDataOuterNode)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    Node newNode{ .value = "ABC" };
    result = api.changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_NODE_NOT_ARRAY);
}


TEST_F(ApiTestError, emptyApi_1)
{
    JsonApi api;
    Node newNode{ .value = "ABC" };
    bool result = api.changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_EMPTY);
}


TEST_F(ApiTestError, emptyApi_2)
{
    JsonApi api;
    std::string json = api.parseObjectToJsonString();
    ASSERT_TRUE(json.empty());
    ASSERT_EQ(api.getErrorCode(), ErrorCode::API_EMPTY);
}

