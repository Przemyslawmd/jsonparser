
#ifndef JX_TEST_XML_BASE_H
#define JX_TEST_XML_BASE_H

#include <fstream>

#include <gtest/gtest.h>

#include "reader/xml/objectCreator.h"
#include "reader/xml/elem.h"
#include "reader/xml/preparser.h"
#include "reader/xml/parserTokensXML.h"
#include "log/ErrorStorage.h"

#include "utilsTest.h"
#include "config.h"
#include "timeType.h"


namespace xml
{
class TestBaseXML : public testing::Test
{
protected:

    TestBaseXML()
    {
        const auto fullPath = std::string(TEST_DATA) + "performance.txt";
        performace.open(fullPath, std::ios::app);
    }

    ~TestBaseXML()
    {
        performace.close();
    }
    
    virtual void SetUp()
    {
        ErrorStorage::clear();
    }

    std::unique_ptr<std::vector<xml::Token>> createTokens(const std::string& path, const std::string& file)
    {
        std::string xmlString = getJsonFromFile(path, file);
        auto preparser = std::make_unique<Preparser>();
        return preparser->parseXML(xmlString);
    }
    
    std::unique_ptr<std::vector<ElemReader>> createElements(const std::string& path, const std::string& file)
    {
        auto tokens = createTokens(path, file);;
        auto parser = std::make_unique<ParserTokens>();
        return parser->parseTokens(std::move(tokens));
    }

    std::unique_ptr<ObjectNode> createObjects(const std::string& path, const std::string& file, KeyMapper& keyMapper)
    {
        auto elems = createElements(path, file);
        auto objCreator = std::make_unique<ObjectCreator>(keyMapper);
        auto node = objCreator->parseElems(*elems);
        return node;
    }

    void showDuration(const TIME_TYPE start, const TIME_TYPE end)
    {
        const char* testCase = ::testing::UnitTest::GetInstance()->current_test_info()->test_case_name();
        const char* testName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\n############ " << testCase << " : " << testName << " : time: " << elapsed.count() << std::endl << std::endl;
        performace << std::left << std::setw(20) << testCase << std::setw(35) << testName << "time: " << elapsed.count() << std::endl;
    }

private:
    std::ofstream performace;
};
}

#endif

