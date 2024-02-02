
#include "../src/Preparser.h"
#include "config.h"

#include <fstream>

#include <gtest/gtest.h>


TEST (PreparserTest, FirstTest)
{
    std::unique_ptr<Preparser> preparser = std::make_unique<Preparser>();

    std::string filePath = std::string(TEST_DATA) + "/test.json";
    std::ifstream jsonFile(filePath);
    std::string jsonStr((std::istreambuf_iterator<char>(jsonFile)),
                         std::istreambuf_iterator<char>());
    preparser->parseJSON(jsonStr);
}

