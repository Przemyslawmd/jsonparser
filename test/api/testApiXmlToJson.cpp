
#include <gtest/gtest.h>

#include "../paths.h"
#include "../baseTest.h"
#include "../utilsTest.h"

#include "src/jsonApi.h"


namespace
{
    class ApiXmlToJsonTest : public BaseTest {};
}

TEST_F(ApiXmlToJsonTest, Test_1)
{
    const auto api = prepareApiWithXml("test_3_attr_1.xml");
    const auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    const std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, "test_from_xml_3_attr_1.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

TEST_F(ApiXmlToJsonTest, Test_2)
{
    const auto api = prepareApiWithXml("test_4_no_declaration.xml");
    const auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    const std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, "test_from_xml_4_no_declaration.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

TEST_F(ApiXmlToJsonTest, Test_Number_Content)
{
    const auto api = prepareApiWithXml("test_content_number_value.xml");
    const auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    const std::string jsonExpected = getContentFromFile(TEST_DATA_JSON, "test_from_xml_number_content.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

