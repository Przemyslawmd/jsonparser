
#ifndef JSONPARSER_TEST_BASE_API_H
#define JSONPARSER_TEST_BASE_API_H

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"


class ApiTest : public testing::Test
{
protected:

    std::unique_ptr<JsonApi> prepareApi(const std::string& file)
    {
        std::string jsonString = TestUtils::getJsonFromFile(TEST_DATA, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseJsonString(jsonString);
        EXPECT_TRUE(result);
        return api;
    }

    const std::string_view PREFIX = "             ###### microseconds: ";
};

#endif

