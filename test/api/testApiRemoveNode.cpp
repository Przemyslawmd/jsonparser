
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
    const auto api = prepareApiWithJson("test_2.json");

    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);
    result = api->removeNodeFromObject({ "shipTo" }, "address");
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromObject)
{
    const auto api = prepareApiWithJson("test_3.json");

    const bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);
    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectFromObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectAndAddForTheSameKey)
{
    const auto api = prepareApiWithJson("test_3.json");

    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);
    result = api->addNodeIntoObject({}, "person", { "newPerson" });
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectAndAddForTheSameKey_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromObject)
{
    const auto api = prepareApiWithJson("test_7.json");
    const bool result = api->removeNodeFromObject({ "employees", static_cast<uint>(1) }, "data");
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeArrayFromObject_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveSimpleNodeFromArray)
{
    const auto api = prepareApiWithJson("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", static_cast<uint>(0), "data", static_cast<uint>(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", static_cast<uint>(0), "data", static_cast<uint>(1) }, 0);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", static_cast<uint>(1), "data", static_cast<uint>(0) }, 1);
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeSimpleNodeFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveObjectFromArray)
{
    const auto api = prepareApiWithJson("test_6.json");

    const bool result = api->removeNodeFromArray({ "employees" }, 0);
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeObjectFromArray_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveArrayFromArray)
{
    const auto api = prepareApiWithJson("test_7.json");
    const bool result = api->removeNodeFromArray({ "employees", static_cast<uint>(0), "data" }, 1);
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "removeArrayFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiRemoveNode, RemoveInComplexJson)
{
    const auto api = prepareApiWithJson("test_8_complex.json");
    const bool result = api->removeNodeFromArray({ "employees",
                                                    static_cast<uint>(1),
                                                    "data",
                                                    static_cast<uint>(2),
                                                    static_cast<uint>(0),
                                                    "numbers" },
                                                     1);
    ASSERT_TRUE(result);

    const std::string json = api->objectToJsonString().value();
    const std::string jsonExpected = getContentFromFile(TEST_DATA_API, "test_8_complex_removed_int.json");
    ASSERT_EQ(json, jsonExpected);
}

