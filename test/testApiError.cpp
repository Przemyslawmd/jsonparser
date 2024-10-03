
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "../src/log/ErrorStorage.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


class ApiTestError : public testing::Test
{
protected:

    std::unique_ptr<JsonApi> api;

    virtual void SetUp()
    {
        api = std::make_unique<JsonApi>();
    }

    virtual void TearDown()
    {
        api.reset();
    }
};


TEST_F(ApiTestError, parseObjectForEmptyRoot)
{
    std::optional<std::string> json = api->parseJsonObjectToString();
    ASSERT_TRUE(json == std::nullopt);
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_EMPTY);
}


TEST_F(ApiTestError, parseJSONStringWithDoubleKey)
{
    std::string jsonString = getJsonFromFile(TEST_DATA_IMPROPER, "double_key.json");
    bool result = api->parseJsonString(jsonString);
    ASSERT_FALSE(result);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::KEY_MAPPER_KEY_STR_REPEAT);
}

