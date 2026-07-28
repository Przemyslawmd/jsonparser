
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "log/ErrorStorage.h"

#include "config.h"
#include "baseTest.h"
#include "utilsTest.h"


class ApiJsonToXmlTest : public BaseTest {};


TEST_F(ApiJsonToXmlTest, Test_1)
{
    auto api = prepareApiWithJson("test_from_xml_4_no_declaration.json");
    auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_4_no_declaration.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_Number_Content)
{
    auto api = prepareApiWithJson("test_from_xml_number_content.json");
    auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_content_number_value.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_No_Root_In_Json)
{
    auto api = prepareApiWithJson("test_5.json");
    auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_5_from_json_root_added.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_No_Root_In_Json_No_Root_In_Settings)
{
    Settings::setXmlRoot(false);
    auto api = prepareApiWithJson("test_5.json");
    auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_5_from_json_no_root.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}
