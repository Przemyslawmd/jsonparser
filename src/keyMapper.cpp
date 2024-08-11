
#include <limits>

#include "keyMapper.h" 

constexpr uint32_t TWO_BYTES = 16;


uint32_t KeyMapper::createAndPutKeyID(const std::string& keyStr, uint32_t mapID)
{
    mapID &= MASK_MAP_ID;
    uint32_t keyID = createKeyID(mapID);
    keyMap.insert(std::make_pair(keyID, keyStr));
    return keyID;
}


void KeyMapper::clear()
{
    keyMap.clear();
}


void KeyMapper::removeKey(uint32_t keyID)
{
    keyMap.erase(keyID);
}


std::optional<std::string> KeyMapper::getKeyStr(uint32_t keyID) const
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

/*******************************************************************/
/* PRIVATE *********************************************************/

uint32_t KeyMapper::createKeyID(uint32_t mapID) const
{
    uint32_t nextItemID = getMaxItemID(mapID) + 1;
    return (mapID & MASK_MAP_ID) + nextItemID;
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

