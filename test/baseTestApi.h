
#ifndef JSONPARSER_TEST_BASE_API_H
#define JSONPARSER_TEST_BASE_API_H

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "utils.h"
#include "config.h"
#include "timeType.h"


class ApiTest : public testing::Test
{
protected:

    std::unique_ptr<JsonApi> prepareApi(const std::string& file)
    {
        std::string jsonString = getJsonFromFile(TEST_DATA, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseJsonString(jsonString);
        EXPECT_TRUE(result);
        return api;
    }

    void showDuration(const TIME_TYPE start, const TIME_TYPE end)
    {
        const char* testCase = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        const char* testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << std::setw(20) << testCase << std::setw(12) << testName << " : time in microseconds : " << elapsed.count() << std::endl;
    }
};

#endif

