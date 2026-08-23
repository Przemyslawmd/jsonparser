
#include "testBaseXML.h"

#include <memory>
#include <variant>
#include <vector>

#include <gtest/gtest.h>


using namespace xml;
using enum TokenType;

namespace
{
    class TestPreparserXML : public BaseTestXML
    {
    protected:
        std::unique_ptr<std::vector<Token>> testPreparser(const std::string& path, const std::string& file)
        {
            const std::string xmlString = getContentFromFile(path, file);
            const auto preparser = std::make_unique<Preparser>();

            const auto begin = std::chrono::high_resolution_clock::now();
            auto tokens = preparser->parseXML(xmlString);
            const auto end = std::chrono::high_resolution_clock::now();

            showDuration(begin, end);
            return tokens;
        }
    };
}

static void checkTokens(const std::vector<Token>& tokens, const std::vector<Token>& testData)
{
    ASSERT_EQ(tokens.size(), testData.size());

    using enum TokenType;
    for (const auto& [a, b] : std::views::zip(tokens, testData)) {
        ASSERT_EQ(a.type, b.type);
        if (a.type == DATA_INT) {
            ASSERT_EQ(std::get<int64_t>(a.data), std::get<int64_t>(b.data));
        }
        else if (a.type == DATA_STR) {
            ASSERT_EQ(std::get<std::string>(a.data), std::get<std::string>(b.data));
        }
        else if (a.type == DATA_DOUBLE) {
            ASSERT_TRUE((std::get<double>(a.data) - std::get<double>(b.data)) <= DBL_EPSILON);
        }
    }
}


TEST_F(TestPreparserXML, Test_File_1)
{
    const auto tokens = testPreparser(TEST_DATA_XML, "test_1.xml");
    const std::vector<Token> testData =
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
    checkTokens(*tokens, testData);
}


TEST_F(TestPreparserXML, Test_One_Letter)
{
    const auto tokens = testPreparser(TEST_DATA_XML, "test_one_letter.xml");
    const std::vector<Token> testData =
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
    checkTokens(*tokens, testData);
}


TEST_F(TestPreparserXML, Error_String_Not_Ended)
{
    const auto tokens = testPreparser(TEST_DATA_IMPROPER_XML, "notEnd.xml");
    ASSERT_EQ(tokens, nullptr);
    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::XML_PREPARSER_STRING_ERROR);
}

