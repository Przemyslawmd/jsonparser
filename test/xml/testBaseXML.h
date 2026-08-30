
#pragma once

#include <gtest/gtest.h>

#include "src/reader/xml/arrayChecker.h"
#include "src/reader/xml/objectCreator.h"
#include "../../src/headers/elem.h"
#include "src/reader/xml/preparser.h"
#include "src/reader/xml/parserTokens.h"
#include "src/writer/xml/elemWriterCreator.h"

#include "baseTest.h"
#include "utilsTest.h"


namespace xml
{
class BaseTestXML : public BaseTest
{
protected:
    void SetUp() override
    {
        ErrorStorage::clear();
        keyMapper = std::make_unique<KeyMapper>();
    }

    void TearDown() override
    {
        keyMapper.reset();
   }

    std::unique_ptr<std::vector<Token>> createTokens(const std::string& path, const std::string& file)
    {
        const std::string xmlString = getContentFromFile(path, file);
        const auto preparser = std::make_unique<Preparser>();
        return preparser->parseXML(xmlString);
    }

    std::unique_ptr<std::vector<Elem>> createElements(const std::string& path, const std::string& file)
    {
        auto tokens = createTokens(path, file);;
        const auto parser = std::make_unique<ParserTokens>();
        return parser->parseTokens(std::move(tokens));
    }

    std::unique_ptr<ObjectNode> createObjects(const std::string& path, const std::string& file)
    {
        const auto elems = createElements(path, file);
        checkArrays(*elems);
        const auto objCreator = std::make_unique<ObjectCreator>(*keyMapper);
        auto node = objCreator->parseElems(*elems);
        return node;
    }

    std::unique_ptr<std::vector<Elem>> createElemsWriter(const std::string& file)
    {
        const auto root = createObjects(TEST_DATA_XML, file);
        const auto writer = std::make_unique<ElemWriterCreator>(*keyMapper);
        return writer->createElems(*root);
    }

    static void typeAndName(const Elem& elem, const ElemType type, const std::string& name)
    {
        ASSERT_EQ(elem.type, type);
        ASSERT_EQ(elem.name, name);
    }

    template <typename T>
    static void typeAndValue(const Elem& elem, const ElemType type, const T& value)
    {
        ASSERT_EQ(elem.type, type);
        ASSERT_EQ(std::get<T>(elem.value), value);
    }

    std::unique_ptr<KeyMapper> keyMapper;
};
}

