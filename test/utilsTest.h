
#ifndef JSONPARSER_TEST_UTILS_H
#define JSONPARSER_TEST_UTILS_H


#include <fstream>
#include <string>


static std::string getJsonFromFile(const std::string& path, const std::string& file)
{
    std::ifstream jsonStream(path + file);
    return { (std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>() };
}

#endif

