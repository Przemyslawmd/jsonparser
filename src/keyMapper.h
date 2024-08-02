
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include <map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    void putKey(const std::string& key, size_t mapID);
    size_t putKeyIntoExistingMap(const std::string& keyStr, size_t mapID);

    std::optional<std::string> getStrKey(const size_t mapID);
    std::optional<size_t> getIdKey(const std::string& keyStr, size_t mapID);

    size_t getMaxMapID();

private:
    std::map<size_t, std::string> keyMap;

    const size_t BIT_MASK = 0b11111111111111110000000000000000;
};

#endif

