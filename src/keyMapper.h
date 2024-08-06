
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include <map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    void putKey(const std::string& key, uint32_t mapID);
    uint32_t putKeyIntoMapAndReturnKeyID(const std::string& keyStr, uint32_t mapID);

    void clear();
    void removeKey(uint32_t keyID);

    std::optional<std::string> getStrKey(uint32_t mapID) const;
    std::optional<uint32_t> getKeyID(const std::string& keyStr, uint32_t mapID) const;

    uint32_t getNextMapID() const;
    uint32_t createItemID(uint32_t mapID, uint32_t nodeID) const;

private:
    std::map<uint32_t, std::string> keyMap;

    const uint32_t MAP_ID_MASK = 0xFFFF0000;
};

#endif

