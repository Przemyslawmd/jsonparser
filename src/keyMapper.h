
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include <cstdint>
#include <map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    std::optional<uint32_t> createAndPutKeyID(const std::string& keyStr, uint32_t mapID);

    void clear();
    void removeKey(uint32_t keyID);

    std::optional<std::string> getKeyStr(uint32_t keyID) const;
    std::optional<uint32_t> getKeyID(const std::string& keyStr, uint32_t mapID) const;

    uint32_t getNextMapID() const;

private:
    uint32_t getMaxItemID(uint32_t mapID) const;

    std::map<uint32_t, std::string> keyMap;
};

#endif

