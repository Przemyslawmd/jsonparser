
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
        if (!tokens || tokens->empty()) {
            return nullptr;
        }
        return tokens;
    }
};


void checkTokens(std::unique_ptr<std::vector<TokenXML>> tokens, std::vector<TokenXML>& testData)
{
    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->size(), testData.size());

    using enum TokenTypeXML;
    for (int i = 0; i < testData.size(); i++) {
        ASSERT_EQ(tokens->at(i).type, testData[i].type );
        if (tokens->at(i).type == DATA_INT) {
            ASSERT_EQ(std::get<int64_t>(tokens->at(i).data), std::get<int64_t>(testData[i].data));
        }
        else if (tokens->at(i).type == DATA_STR) {
            ASSERT_EQ(std::get<std::string>(tokens->at(i).data), std::get<std::string>(testData[i].data));
        }
        else if (tokens->at(i).type == DATA_DOUBLE) {
            ASSERT_TRUE((std::get<double>(tokens->at(i).data) - std::get<double>(testData[i].data)) <= DBL_EPSILON);
        }
    }
}


TEST_F(TestPreparserXML, Test_File_1)
{
    auto tokens = createTokens(TEST_DATA_XML, "test_1.xml");
    std::vector<TokenXML> testData =
    {
        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::QUESTION },
        { TokenTypeXML::DATA_STR, std::string{ "xml" }},
        { TokenTypeXML::DATA_STR, std::string{ "version" }},
        { TokenTypeXML::EQUAL },
        { TokenTypeXML::DATA_STR_QUOTA, std::string{ "1.0" }},
        { TokenTypeXML::DATA_STR, std::string{ "encoding" }},
        { TokenTypeXML::EQUAL },
        { TokenTypeXML::DATA_STR_QUOTA, std::string{ "UTF-8" }},
        { TokenTypeXML::QUESTION },
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "person" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "name" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_STR, std::string{ "Jan" }},

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "name" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "surname" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_STR, std::string{ "Kowalski" }},

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "surname" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "city" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_STR, std::string{ "Warszawa" }},

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "city" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "number" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_INT, 34567 },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "number" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "result" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_DOUBLE, 12.45 },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "result" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "person" }},
        { TokenTypeXML::ANGLE_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST_F(TestPreparserXML, Test_one_letter)
{
    auto tokens = createTokens(TEST_DATA_XML, "test_one_letter.xml");
    std::vector<TokenXML> testData =
    {
        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::QUESTION },
        { TokenTypeXML::DATA_STR, std::string{ "xml" }},
        { TokenTypeXML::DATA_STR, std::string{ "version" }},
        { TokenTypeXML::EQUAL },
        { TokenTypeXML::DATA_STR_QUOTA, std::string{ "1.0" }},
        { TokenTypeXML::DATA_STR, std::string{ "encoding" }},
        { TokenTypeXML::EQUAL },
        { TokenTypeXML::DATA_STR_QUOTA, std::string{ "UTF-8" }},
        { TokenTypeXML::QUESTION },
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "person" }},
        { TokenTypeXML::DATA_STR, std::string{ "data" }},
        { TokenTypeXML::EQUAL },
        { TokenTypeXML::DATA_STR_QUOTA, std::string{ "x" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::DATA_STR, std::string{ "name" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::DATA_STR, std::string{ "J" }},

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "name" }},
        { TokenTypeXML::ANGLE_CLOSE },

        { TokenTypeXML::ANGLE_OPEN },
        { TokenTypeXML::SLASH },
        { TokenTypeXML::DATA_STR, std::string{ "person" }},
        { TokenTypeXML::ANGLE_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}

