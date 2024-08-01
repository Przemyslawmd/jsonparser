
#include <limits>

#include "keyMapper.h" 


void KeyMapper::putKey(const std::string& key, size_t id)
{
    if (keyMap.contains(id)) {
        return;
    }
    keyMap.insert(std::make_pair(id, key));
}


std::optional<std::string> KeyMapper::getStrKey(const size_t id)
{
    if (keyMap.contains(id) == false) {
        return std::nullopt;
    }
    return keyMap.at(id);
}


std::optional<size_t> KeyMapper::getIdKey(const std::string& val, size_t mapId)
{
    const size_t BIT_MASK = 0b11111111111111110000000000000000;
    for (const auto& [key, value] : keyMap) {
        if (value.compare(val) == 0 && ((key & BIT_MASK) == (mapId & BIT_MASK))) {
            return key;
        }
    }
    return std::nullopt;
}

