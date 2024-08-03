
#include <limits>

#include "keyMapper.h" 

constexpr uint32_t TWO_BYTES = 16;


void KeyMapper::putKey(const std::string& keyStr, uint32_t keyID)
{
    if (keyMap.contains(keyID)) {
        return;
    }
    keyMap.insert(std::make_pair(keyID, keyStr));
}


uint32_t KeyMapper::putKeyIntoMapAndReturnKeyID(const std::string& keyStr, uint32_t mapID)
{
    size_t maxKeyID = 0;
    for (const auto& [key, value] : keyMap) {
        if ((key & MAP_ID_MASK) == (mapID & MAP_ID_MASK)) {
            maxKeyID = key > maxKeyID ? key : maxKeyID;
        }
    }
    putKey(keyStr, maxKeyID + 1);
    return maxKeyID + 1;
}


void KeyMapper::removeKey(uint32_t keyID)
{
    keyMap.erase(keyID);
}


std::optional<std::string> KeyMapper::getStrKey(uint32_t keyID) const
{
    if (keyMap.contains(keyID) == false) {
        return std::nullopt;
    }
    return keyMap.at(keyID);
}


std::optional<uint32_t> KeyMapper::getKeyID(const std::string& keyStr, uint32_t mapID) const
{
    for (const auto& [key, value] : keyMap) {
        if (value.compare(keyStr) == 0 && ((key & MAP_ID_MASK) == (mapID & MAP_ID_MASK))) {
            return key;
        }
    }
    return std::nullopt;
}


uint32_t KeyMapper::createItemID(uint32_t mapID, uint32_t nodeID) const
{
    return (mapID & MAP_ID_MASK) + nodeID;
}


uint32_t KeyMapper::getNextMapID() const
{
    size_t maxMapID = 0;
    for (const auto& [key, value] : keyMap) {
        if (key > maxMapID) {
            maxMapID = key;
        }
    }
    return (maxMapID & MAP_ID_MASK) + (uint32_t(1) << TWO_BYTES);
}

