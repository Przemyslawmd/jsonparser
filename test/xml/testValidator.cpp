
#include <memory>

#include <gtest/gtest.h>

#include "reader/xml/PreparserXML.h"
#include "reader/xml/ParserTokensXML.h"
#include "reader/xml/item.h"

#include "error.h"
#include "log/ErrorStorage.h"
#include "reader/xml/Validator.h"

#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestValidatorXML : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Elem>> createElements(const std::string& path, const std::string& file)
    {
        ErrorStorage::clear();
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<PreparserXML>();
        auto tokens = preparser->parseXML(xmlString);
        auto parser = std::make_unique<ParserTokens>();
        return parser->parseTokens(std::move(tokens));
    }
};


TEST_F(TestValidatorXML, Declaration_not_start)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "declaration_not_start.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_DECLARATION_NOT_START);
}


TEST_F(TestValidatorXML, MismatchedTag_1)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_1.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_2)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_2.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_3)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_3.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}


TEST_F(TestValidatorXML, MismatchedTag_4)
{
    auto elems = createElements(TEST_DATA_IMPROPER_XML, "mismatchedTag_4.xml");
    ASSERT_NE(elems, nullptr);
    bool res = ValidateElems(*elems);
    ASSERT_FALSE(res);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
}

