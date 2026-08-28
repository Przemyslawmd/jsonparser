
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "settings.h"

#include "../paths.h"
#include "../baseTest.h"
#include "../utilsTest.h"


namespace
{
    class ApiJsonToXmlTest : public BaseTest {};
}


TEST_F(ApiJsonToXmlTest, Test_1)
{
    const auto api = prepareApiWithJson("test_from_xml_4_no_declaration.json");
    const auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_4_no_declaration.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_2_Number_Content)
{
    const auto api = prepareApiWithJson("test_from_xml_number_content.json");
    const auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_content_number_value.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_3_No_Json_Root)
{
    const auto api = prepareApiWithJson("test_5.json");
    const auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_5_from_json_root_added.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}


TEST_F(ApiJsonToXmlTest, Test_4_No_Json_Root_No_Settings_Root)
{
    Settings::setXmlRoot(false);
    const auto api = prepareApiWithJson("test_5.json");
    const auto xmlString = api->objectToXmlString();
    ASSERT_TRUE(xmlString.has_value());
    const std::string xmlExpected = getContentFromFile(TEST_DATA_XML, "test_5_from_json_no_root.xml");
    ASSERT_EQ(xmlString.value(), xmlExpected);
}
