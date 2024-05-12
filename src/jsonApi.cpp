
#include "jsonApi.h"

#include <iostream>

#include "../src/reader/Parser.h"
#include "../src/reader/ParserKey.h"
#include "../src/reader/Preparser.h"
#include "../src/reader/Validator.h"
#include "../src/writer/Writer.h"


bool jsonApi::parseJsonString(const std::string& jsonString)
{
    const auto preparser = std::make_unique<Preparser>();
    auto tokens = preparser->parseJSON(jsonString);
    if (tokens == nullptr) {
        result = preparser->getError();
        return false;
    }

    const auto validator = std::make_unique<Validator>();
    result = validator->validate(*tokens);
    if (result != Result::OK) {
        return false;
    }

    const auto parserKey = std::make_unique<ParserKey>();
    tokens = parserKey->createKeyTokens(std::move(tokens));

    const auto parser = std::make_unique<Parser>();
    root = parser->parseTokens(*tokens);
    return true;
}


std::string jsonApi::parseObjectToJsonString(ObjectNode* node)
{
    auto writer = std::make_unique<Writer>();
    std::string jsonString = writer->createJsonString(node);
    return jsonString;
}


ObjectNode* jsonApi::getRoot()
{
    return root.get();
}


Result jsonApi::getLastError()
{
    return result;
}



