
#ifndef JX_TEST_UTILS_H
#define JX_TEST_UTILS_H


#include <fstream>
#include <string>


static std::string getContentFromFile(const std::string& path, const std::string& file)
{
    std::ifstream contentStream(path + file);
    return { (std::istreambuf_iterator<char>(contentStream)), std::istreambuf_iterator<char>() };
}

#endif

