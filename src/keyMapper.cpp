
#include "keyMapper.h" 

#include <ranges>

#include "log/ErrorStorage.h"


constexpr uint32_t TWO_BYTES = 16;
constexpr uint32_t MASK_MAP_ID = 0xFF'FF'00'00;
constexpr uint32_t MASK_ITEM_ID = 0x00'00'FF'FF;


std::optional<uint32_t>
KeyMapper::createAndPutKeyID(std::string_view keyStr, uint32_t mapID)
{
    mapID &= MASK_MAP_ID;
    if (getKeyID(keyStr, mapID) != std::nullopt) {
        ErrorStorage::putError(ErrorCode::KEY_MAPPER_KEY_STR_REPEAT);
        return std::nullopt;
    }
    uint32_t newKeyID = mapID + getMaxItemID(mapID) + 1;
    keyMap.emplace(newKeyID, keyStr);
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
KeyMapper::getKeyID(std::string_view keyString, uint32_t mapID) const
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
    for (const auto keyID : keyMap | std::views::keys) {
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
    for (const auto keyID : keyMap | std::views::keys) {
        if ((keyID & MASK_MAP_ID) == mapID && (keyID & MASK_ITEM_ID) > maxItemID) {
            maxItemID = (keyID & MASK_ITEM_ID);
        }
    }
    return maxItemID;
}

