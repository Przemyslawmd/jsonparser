
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


std::optional<size_t> KeyMapper::getIdKey(const std::string& val, size_t mapId)
{
    for (const auto& [key, value] : keyMap) {
        if (value.compare(val) == 0 && ((key & BIT_MASK) == (mapId & BIT_MASK))) {
            return key;
        }
    }
    return std::nullopt;
}

