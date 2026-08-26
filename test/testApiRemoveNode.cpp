
#include <chrono>

#include <gtest/gtest.h>

#include "baseTest.h"
#include "paths.h"

#include "src/jsonApi.h"


using std::chrono::high_resolution_clock;
using uint = unsigned int;


namespace
{
    class ApiRemoveNode : public BaseTest {};
}

TEST_F(ApiRemoveNode, RemoveSimpleNodeFromObject)
{
    auto api = prepareApiWithJson("test_2.json");

    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);
    result = api->removeNodeFromObject({ "shipTo" }, "address");
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromObject)
{
    auto api = prepareApiWithJson("test_3.json");

    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);
    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectFromObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectAndAddForTheSameKey)
{
    auto api = prepareApiWithJson("test_3.json");

    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);
    result = api->addNodeIntoObject({}, "person", { "newPerson" });
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectAndAddForTheSameKey_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromObject)
{
    auto api = prepareApiWithJson("test_7.json");
    bool result = api->removeNodeFromObject({ "employees", uint(1) }, "data");
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeArrayFromObject_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveSimpleNodeFromArray)
{
    auto api = prepareApiWithJson("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", uint(0), "data", uint(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", uint(0), "data", uint(1) }, 0);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", uint(1), "data", uint(0) }, 1);
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeSimpleNodeFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromArray)
{
    auto api = prepareApiWithJson("test_6.json");

    bool result = api->removeNodeFromArray({ "employees" }, 0);
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectFromArray_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromArray)
{
    auto api = prepareApiWithJson("test_7.json");
    bool result = api->removeNodeFromArray({ "employees", uint(0), "data" }, 1);
    ASSERT_TRUE(result);

    std::string json = api->objectToJsonString().value();
    std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeArrayFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

