
#include <memory>
#include <variant>
#include <vector>

#include <gtest/gtest.h>

#include "reader/xml/PreparserXML.h"
#include "../headers/token.h"
#include "../baseTest.h"
#include "../config.h"


class TestPreparserXML : public BaseTest
{
protected:
    std::unique_ptr<std::vector<TokenXML>> createTokens(const std::string& path, const std::string& file)
    {
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<PreparserXML>();
        auto tokens = preparser->parseXML(xmlString);
        if (!tokens) {
            return nullptr;
        }
        return nullptr;
    }
};


struct TestData 
{
    TokenTypeXML type;
    std::variant<std::string, int64_t, double, bool, nullptr_t> data;
};


TEST_F(TestPreparserXML, Test_File_1)
{
    auto tokens = createTokens(TEST_DATA, "test_1.xml");
    ASSERT_FALSE(tokens);
}

