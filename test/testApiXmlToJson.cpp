
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "log/ErrorStorage.h"

#include "config.h"
#include "baseTest.h"
#include "utilsTest.h"


class ApiXmlToJsonTest : public BaseTest {};


TEST_F(ApiXmlToJsonTest, Test_1)
{
    auto api = prepareApiWithXml("test_3_attr_1.xml");
    auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, "test_from_xml_3_attr_1.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

TEST_F(ApiXmlToJsonTest, Test_2)
{
    auto api = prepareApiWithXml("test_4_no_declaration.xml");
    auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, "test_from_xml_4_no_declaration.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

