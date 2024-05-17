
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

    Node newNode;
    newNode.value = "Spain";
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

    Node newNode;
    newNode.value = "Spain";
    result = api.changeNodeValue({ "employees", size_t(3), "data", size_t(3), size_t(2) }, newNode);
    ASSERT_FALSE(result);
    Result s = api.getLastError();
    ASSERT_EQ(api.getLastError(), Result::API_INDEX_OUT_OF_ARRAY);
}

