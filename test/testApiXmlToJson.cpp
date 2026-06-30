
#include <gtest/gtest.h>

#include "jsonApi.h"
#include "log/ErrorStorage.h"

#include "config.h"
#include "baseTest.h"
#include "utilsTest.h"
#include "node.h"


class ApiXmlToJsonTest : public BaseTest {};


TEST_F(ApiXmlToJsonTest, Test_1)
{
    auto api = prepareApiWithXml("test_3_attr_1.xml");
    auto jsonString = api->objectToJsonString();
    ASSERT_TRUE(jsonString.has_value());
    std::string jsonExpected = getJsonFromFile(TEST_DATA_JSON, "test_from_xml_3_attr_1.json");
    ASSERT_EQ(jsonString.value(), jsonExpected);
}

