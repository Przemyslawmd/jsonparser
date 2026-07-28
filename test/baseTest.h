
#pragma once

#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "utilsTest.h"
#include "config.h"
#include "timeType.h"


class BaseTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        ErrorStorage::clear();
        checkDuration = false;
    }

    std::unique_ptr<JsonApi> prepareApiWithJson(const std::string& file)
    {
        std::string jsonString = getContentFromFile(TEST_DATA_JSON, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseJsonString(jsonString);
        EXPECT_TRUE(result);
        return api;
    }

    std::unique_ptr<JsonApi> prepareApiWithXml(const std::string& file)
    {
        std::string xmlString = getContentFromFile(TEST_DATA_XML, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseXmlString(xmlString);
        EXPECT_TRUE(result);
        return api;
    }

    void showDuration(const TIME_TYPE start, const TIME_TYPE end)
    {
        if (!checkDuration) {
            return;
        }
        const char* testCase = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        const char* testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\n############ " << testCase << " : " << testName << " : time: " << elapsed.count() << std::endl << std::endl;
        const auto fullPath = std::string(TEST_DATA) + "performance.txt";
        performace.open(fullPath, std::ios::app);
        performace << std::left << std::setw(20) << testCase << std::setw(35) << testName << "time: " << elapsed.count() << std::endl;
        performace.close();
    }

    bool checkDuration;

private:
    std::ofstream performace;
};

