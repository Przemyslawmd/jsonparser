
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "log/ErrorStorage.h"

#include "paths.h"
#include "utilsTest.h"


namespace
{
    class ApiTestError : public testing::Test
    {
    protected:

        std::unique_ptr<JsonApi> api;

        void SetUp() override
        {
            api = std::make_unique<JsonApi>();
        }

        void TearDown() override
        {
            api.reset();
        }
    };
}


TEST_F(ApiTestError, parseObjectForEmptyRoot)
{
    std::optional<std::string> json = api->objectToJsonString();
    ASSERT_FALSE(json.has_value());
    const auto& errors = api->getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_NO_OBJECT);
}


TEST_F(ApiTestError, parseJSONStringWithDoubleKey)
{
    std::string jsonString = getContentFromFile(TEST_DATA_IMPROPER_JSON, "double_key.json");
    ASSERT_FALSE(api->parseJsonString(jsonString));
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::KEY_MAPPER_KEY_STR_REPEAT);
}

