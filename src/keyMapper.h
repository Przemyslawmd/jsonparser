
#ifndef JX_KEY_MAPPER_H
#define JX_KEY_MAPPER_H

#include <cstdint>
#include <map> 
#include <optional>
#include <string>
#include <string_view>
#include <vector>


class KeyMapper
{
public:
    std::optional<uint32_t> createKeyID(std::string_view keyStr, uint32_t mapID);
    std::optional<uint32_t> createKeyIDAttr(std::string_view keyStr, uint32_t mapID);

    void clear();
    void removeKey(uint32_t keyID);
    bool isAttrKey(uint32_t keyID) const;

    std::optional<std::string> getKeyStr(uint32_t keyID) const;
    std::optional<uint32_t> getKeyID(std::string_view keyStr, uint32_t mapID) const;

    uint32_t getNextMapID() const;

private:
    uint32_t getMaxItemID(uint32_t mapID) const;

    std::map<uint32_t, std::string> keyMap;
    std::vector<uint32_t> attrs;
};

#endif

