
#pragma once

#include <gtest/gtest.h>

#include "reader/xml/objectCreator.h"
#include "reader/xml/elem.h"
#include "reader/xml/preparser.h"
#include "reader/xml/parserTokens.h"
#include "log/ErrorStorage.h"
#include "settings.h"

#include "baseTest.h"
#include "utilsTest.h"
#include "config.h"


namespace xml
{
class BaseTestXML : public BaseTest
{
protected:
    std::unique_ptr<std::vector<Token>> createTokens(const std::string& path, const std::string& file)
    {
        std::string xmlString = getContentFromFile(path, file);
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
};
}

