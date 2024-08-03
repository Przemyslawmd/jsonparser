
#ifndef JSONPARSER_KEY_MAPPER_H
#define JSONPARSER_KEY_MAPPER_H

#include <map> 
#include <optional>
#include <string>


class KeyMapper
{
public:
    void putKey(const std::string& key, size_t mapID);
    size_t putKeyIntoMapAndReturnKeyID(const std::string& keyStr, size_t mapID);

    void removeKey(size_t keyID);

    std::optional<std::string> getStrKey(const size_t mapID);
    std::optional<size_t> getKeyID(const std::string& keyStr, size_t mapID);

    size_t getNextMapID() const;
    size_t createItemID(size_t mapID, size_t nodeID) const;

private:
    std::map<size_t, std::string> keyMap;

    const size_t MAP_ID_MASK = 0xFFFF0000;
};

#endif

