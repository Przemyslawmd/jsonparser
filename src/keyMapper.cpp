
#include <limits>

#include "keyMapper.h" 


void KeyMapper::putKey(const std::string& keyStr, size_t keyID)
{
    if (keyMap.contains(keyID)) {
        return;
    }
    keyMap.insert(std::make_pair(keyID, keyStr));
}


size_t KeyMapper::putKeyIntoExistingMap(const std::string& keyStr, size_t mapID)
{
    size_t maxKeyID = 0;
    for (const auto& [key, value] : keyMap) {
        if ((key & BIT_MASK) == (mapID & BIT_MASK)) {
            maxKeyID = key > maxKeyID ? key : maxKeyID;
        }
    }
    putKey(keyStr, maxKeyID + 1);
    return maxKeyID + 1;
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
        if (value.compare(keyStr) == 0 && ((key & BIT_MASK) == (mapID & BIT_MASK))) {
            return key;
        }
    }
    return std::nullopt;
}


size_t KeyMapper::getMaxMapID()
{
    size_t maxMapID = 0;
    for (const auto& [key, value] : keyMap) {
        if (key > maxMapID) {
            maxMapID = key;
        }
    }
    return maxMapID & BIT_MASK;
}
