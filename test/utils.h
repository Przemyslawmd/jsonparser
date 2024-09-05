
#ifndef JSONPARSER_TEST_UTILS_H
#define JSONPARSER_TEST_UTILS_H


#include <fstream>
#include <string>


class TestUtils
{
public:

    static std::string getJsonFromFile(const std::string& path, const std::string& file)
    {
        std::string filePath = path + file;
        std::ifstream jsonStream(filePath);
        return { (std::istreambuf_iterator<char>(jsonStream)), std::istreambuf_iterator<char>() };
    }
};

#endif

