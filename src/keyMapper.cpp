
#include <limits>

#include "keyMapper.h" 

constexpr size_t TWO_BYTES = 16;

void KeyMapper::putKey(const std::string& keyStr, size_t keyID)
{
    if (keyMap.contains(keyID)) {
        return;
    }
    keyMap.insert(std::make_pair(keyID, keyStr));
}


size_t KeyMapper::putKeyIntoMapAndReturnKeyID(const std::string& keyStr, size_t mapID)
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


void KeyMapper::removeKey(size_t keyID)
{
    keyMap.erase(keyID);
}


std::optional<std::string> KeyMapper::getStrKey(const size_t keyID)
{
    if (keyMap.contains(keyID) == false) {
        return std::nullopt;
    }
    return keyMap.at(keyID);
}


std::optional<size_t> KeyMapper::getKeyID(const std::string& keyStr, size_t mapID)
{
    for (const auto& [key, value] : keyMap) {
        if (value.compare(keyStr) == 0 && ((key & MAP_ID_MASK) == (mapID & MAP_ID_MASK))) {
            return key;
        }
    }
    return std::nullopt;
}


size_t KeyMapper::createItemID(size_t mapID, size_t nodeID) const
{
    return (mapID & MAP_ID_MASK) + nodeID;
}


size_t KeyMapper::getNextMapID() const
{
    size_t maxMapID = 0;
    for (const auto& [key, value] : keyMap) {
        if (key > maxMapID) {
            maxMapID = key;
        }
    }
    return (maxMapID & MAP_ID_MASK) + (size_t(1) << TWO_BYTES);
    uint32_t c;
}
