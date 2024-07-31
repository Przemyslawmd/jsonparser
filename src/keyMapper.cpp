
#include <limits>

#include "keyMapper.h" 


void KeyMapper::putKey(const std::string& key, size_t id)
{
    if (keyMap.contains(id)) {
        return;
    }
    keyMap.insert(std::make_pair(id, key));
}


std::optional<std::string> KeyMapper::getKey(const size_t id)
{
    if (keyMap.contains(id) == false) {
        return std::nullopt;
    }
    return keyMap.at(id);
}

