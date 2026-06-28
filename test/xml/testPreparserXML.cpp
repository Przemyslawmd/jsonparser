
#include "testBaseXML.h"

#include <memory>
#include <variant>
#include <vector>

#include <gtest/gtest.h>


using enum xml::TokenType;

class TestPreparserXML : public TestBaseXML {};


void checkTokens(std::unique_ptr<std::vector<Token>> tokens, std::vector<Token>& testData)
{
    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->size(), testData.size());

    using enum TokenType;
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
    std::vector<Token> testData =
    {
        { ANGLE_OPEN },
        { QUESTION },
        { DATA_STR, std::string{ "xml" }},
        { DATA_STR, std::string{ "version" }},
        { EQUAL },
        { DATA_STR_QUOTA, std::string{ "1.0" }},
        { DATA_STR, std::string{ "encoding" }},
        { EQUAL },
        { DATA_STR_QUOTA, std::string{ "UTF-8" }},
        { QUESTION },
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "person" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "name" }},
        { ANGLE_CLOSE },

        { DATA_STR, std::string{ "Jan" }},

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "name" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "surname" }},
        { ANGLE_CLOSE },

        { DATA_STR, std::string{ "Kowalski" }},

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "surname" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "city" }},
        { ANGLE_CLOSE },

        { DATA_STR, std::string{ "Warszawa" }},

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "city" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "number" }},
        { ANGLE_CLOSE },

        { DATA_INT, 34567 },

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "number" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "result" }},
        { ANGLE_CLOSE },

        { DATA_DOUBLE, 12.45 },

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "result" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "person" }},
        { ANGLE_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}


TEST_F(TestPreparserXML, Test_one_letter)
{
    auto tokens = createTokens(TEST_DATA_XML, "test_one_letter.xml");
    std::vector<Token> testData =
    {
        { ANGLE_OPEN },
        { QUESTION },
        { DATA_STR, std::string{ "xml" }},
        { DATA_STR, std::string{ "version" }},
        { EQUAL },
        { DATA_STR_QUOTA, std::string{ "1.0" }},
        { DATA_STR, std::string{ "encoding" }},
        { EQUAL },
        { DATA_STR_QUOTA, std::string{ "UTF-8" }},
        { QUESTION },
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "person" }},
        { DATA_STR, std::string{ "data" }},
        { EQUAL },
        { DATA_STR_QUOTA, std::string{ "x" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { DATA_STR, std::string{ "name" }},
        { ANGLE_CLOSE },

        { DATA_STR, std::string{ "J" }},

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "name" }},
        { ANGLE_CLOSE },

        { ANGLE_OPEN },
        { SLASH },
        { DATA_STR, std::string{ "person" }},
        { ANGLE_CLOSE },
    };
    checkTokens(std::move(tokens), testData);
}

