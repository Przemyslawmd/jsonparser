
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include<map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    void putKey(const std::string& key, size_t mapId);

    std::optional<std::string> getStrKey(const size_t id);
    std::optional<size_t> getIdKey(const std::string& key, size_t mapId);

private:
    std::map<size_t, std::string> keyMap;
};

#endif

