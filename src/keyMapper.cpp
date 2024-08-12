
#include <limits>

#include "keyMapper.h" 

constexpr uint32_t TWO_BYTES = 16;


std::optional<uint32_t>
KeyMapper::createAndPutKeyID(const std::string& keyStr, uint32_t mapID)
{
    mapID &= MASK_MAP_ID;
    if (getKeyID(keyStr, mapID) != std::nullopt) {
        return std::nullopt;
    }
    uint32_t newKeyID = mapID + getMaxItemID(mapID) + 1;
    keyMap.insert(std::make_pair(newKeyID, keyStr));
    return newKeyID;
}


std::optional<std::string> KeyMapper::getKeyStr(uint32_t keyID) const
{
    if (keyMap.contains(keyID) == false) {
        return std::nullopt;
    }
    return keyMap.at(keyID);
}


std::optional<uint32_t> 
KeyMapper::getKeyID(const std::string& keyString, uint32_t mapID) const
{
    for (const auto& [keyID, keyStr] : keyMap) {
        if (keyStr.compare(keyString) == 0 && ((keyID & MASK_MAP_ID) == (mapID & MASK_MAP_ID))) {
            return keyID;
        }
    }
    return std::nullopt;
}


uint32_t KeyMapper::getNextMapID() const
{
    uint32_t maxMapID = 0;
    for (const auto& [keyID, _] : keyMap) {
        if (keyID > maxMapID) {
            maxMapID = keyID;
        }
    }
    return (maxMapID & MASK_MAP_ID) + (uint32_t(1) << TWO_BYTES);
}


void KeyMapper::clear()
{
    keyMap.clear();
}


void KeyMapper::removeKey(uint32_t keyID)
{
    keyMap.erase(keyID);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

uint32_t KeyMapper::getMaxItemID(uint32_t mapID) const
{
    uint32_t maxItemID = 0;
    for (const auto& [keyID, _] : keyMap) {
        if ((keyID & MASK_MAP_ID) == mapID && (keyID & MASK_ITEM_ID) > maxItemID) {
            maxItemID = (keyID & MASK_ITEM_ID);
        }
    }
    return maxItemID;
}

