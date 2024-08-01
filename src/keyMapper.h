
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include<map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    void putKey(const std::string& key, size_t mapId);

    std::optional<std::string> getKey(const size_t id);

private:
    std::map<size_t, std::string> keyMap;
};

#endif

