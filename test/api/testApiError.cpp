
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "log/ErrorStorage.h"

#include "baseTest.h"
#include "paths.h"
#include "utilsTest.h"


namespace
{
    class ApiTestError : public BaseTest {};
}


TEST_F(ApiTestError, parseObjectForEmptyRoot)
{
    const auto api = std::make_unique<JsonApi>();
    std::optional<std::string> json = api->objectToJsonString();
    ASSERT_FALSE(json.has_value());
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NO_OBJECT);
}


TEST_F(ApiTestError, parseJSONStringWithDoubleKey)
{
    const std::string jsonString = getContentFromFile(TEST_DATA_IMPROPER_JSON, "double_key.json");
    const auto api = std::make_unique<JsonApi>();
    ASSERT_FALSE(api->parseJsonString(jsonString));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::KEY_MAPPER_KEY_STR_REPEAT);
}


TEST_F(ApiTestError, ErrorImproperKeyInPath)
{
    const auto api = prepareApiWithJson("test_4.json");
    NodeApi newNode{ "Cracow" };

    const bool result = api->changeNodeInObject({ "person2", "street" }, "address", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
}


TEST_F(ApiTestError, ErrorImproperKeyInNode)
{
    const auto api = prepareApiWithJson("test_4.json");

    NodeApi newNode{ false };
    const bool result = api->changeNodeInObject({ "person2", "address" }, "bbb", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NOT_KEY_IN_OBJECT);
}


TEST_F(ApiTestError, ErrorOutOfIndexInPath)
{
    const auto api = prepareApiWithJson("test_7.json");

    NodeApi newNode{ "Spain" };
    const bool result = api->changeNodeInArray({ "employees", static_cast<uint>(3), "data", static_cast<uint>(3) },
                                               2,
                                               newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, OutOfIndexInNode)
{
    auto api = prepareApiWithJson("test_7.json");

    NodeApi newNode{ 23.45 };
    const bool result = api->changeNodeInArray({ "employees", static_cast<uint>(0), "data", static_cast<uint>(1) },
                                               4,
                                               newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_INDEX_OUT_OF_ARRAY);
}


TEST_F(ApiTestError, ErrorImproperPath)
{
    const auto api = prepareApiWithJson("test_4.json");

    NodeApi newNode{ 12 };
    const bool result = api->changeNodeInObject({ "person2", static_cast<uint>(0) }, "city", newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_IMPROPER_PATH);
}


TEST_F(ApiTestError, ErrorImproperIndicatorForNode)
{
    const auto api = prepareApiWithJson("test_4.json");

    NodeApi newNode{ "ABC" };
    const bool result = api->changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NODE_NOT_ARRAY);
}


TEST_F(ApiTestError, ErrorEmptyRoot)
{
    const auto api = std::make_unique<JsonApi>();

    NodeApi newNode{ "ABC" };
    const bool result = api->changeNodeInArray({ "person2", "address" }, 1, newNode);
    ASSERT_FALSE(result);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NO_OBJECT);
}

