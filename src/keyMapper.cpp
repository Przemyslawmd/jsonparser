
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
        if ((key & MASK_MAP_ID) == (mapID & MASK_MAP_ID)) {
            maxKeyID = key > maxKeyID ? key : maxKeyID;
        }
    }
    putKey(keyStr, maxKeyID + 1);
    return maxKeyID + 1;
}


void KeyMapper::clear()
{
    keyMap.clear();
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
        if (value.compare(keyStr) == 0 && ((key & MASK_MAP_ID) == (mapID & MASK_MAP_ID))) {
            return key;
        }
    }
    return std::nullopt;
}


uint32_t KeyMapper::createKeyID(uint32_t mapID) const
{
    uint32_t nextItemID = getMaxItemID(mapID) + 1;
    return (mapID & MASK_MAP_ID) + nextItemID;
}


uint32_t KeyMapper::getNextMapID() const
{
    uint32_t maxMapID = 0;
    for (const auto& [key, value] : keyMap) {
        if (key > maxMapID) {
            maxMapID = key;
        }
    }
    return (maxMapID & MASK_MAP_ID) + (uint32_t(1) << TWO_BYTES);
}


uint32_t KeyMapper::getMaxItemID(uint32_t mapID) const
{
    uint32_t maxItemID = 0;
    for (const auto& [keyID, _] : keyMap) {
        if (((keyID & MASK_MAP_ID) == mapID) && ((keyID & MASK_ITEM_ID) > maxItemID)) {
            maxItemID = (keyID & MASK_ITEM_ID);
        }
    }
    return maxItemID;
}

