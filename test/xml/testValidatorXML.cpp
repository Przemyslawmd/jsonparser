
#include <gtest/gtest.h>

#include "error.h"
#include "log/ErrorStorage.h"
#include "reader/xml/PreparserXML.h"
#include "reader/xml/ValidatorXML.h"

#include "config.h"
#include "utilsTest.h"


static void makeValidatorError(const std::string& xmlFile)
{
    ErrorStorage::clear();
    std::string xmlString = getJsonFromFile(TEST_DATA_IMPROPER_XML, xmlFile);
    auto preparser = std::make_unique<PreparserXML>();
    auto tokens = preparser->parseXML(xmlString);
    bool result = validate(*tokens);
    EXPECT_FALSE(result);
}


TEST(ValidatorXMLTest, ImproperBegin)
{
    makeValidatorError("improper_begin.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_TRUE(true);
}


TEST(ValidatorXMLTest, ImproperEnd)
{
    makeValidatorError("improper_end.json");
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_TRUE(true);
}

