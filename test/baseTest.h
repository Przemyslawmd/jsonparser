
#pragma once

#include <fstream>

#include <gtest/gtest.h>

#include "paths.h"
#include "timeType.h"
#include "utilsTest.h"

#include "../src/jsonApi.h"
#include "../src/log/ErrorStorage.h"


class BaseTest : public testing::Test
{
protected:
    BaseTest() = default;

    void SetUp() override
    {
        ErrorStorage::clear();
    }

    static std::unique_ptr<JsonApi> prepareApiWithJson(const std::string& file)
    {
        const std::string jsonString = getContentFromFile(TEST_DATA_JSON, file);
        auto api = std::make_unique<JsonApi>();
        EXPECT_TRUE(api->parseJsonString(jsonString));
        return api;
    }

    static std::unique_ptr<JsonApi> prepareApiWithXml(const std::string& file)
    {
        const std::string xmlString = getContentFromFile(TEST_DATA_XML, file);
        auto api = std::make_unique<JsonApi>();
        EXPECT_TRUE(api->parseXmlString(xmlString));
        return api;
    }

    void showDuration(const TIME_TYPE start, const TIME_TYPE end)
    {
        const char* testCase = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        const char* testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\n############ " << testCase << " : " << testName << " : time: " << elapsed.count() << std::endl << std::endl;
        const auto fullPath = std::string(TEST_DATA) + "performance.txt";
        performance.open(fullPath, std::ios::app);
        performance << std::left << std::setw(24) << testCase << std::setw(25) << testName << "time: " << elapsed.count() << std::endl;
        performance.close();
    }

private:
    std::ofstream performance;
};

