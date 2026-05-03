
#ifndef JSONPARSER_TAG_H
#define JSONPARSER_TAG_H

#include <optional>
#include <string>
#include <variant>
#include <vector>


enum class ItemType
{
    DECLARATION,
    TAG_OPEN,
    TAG_CLOSE,
    VALUE
};


struct Item
{
    ItemType type;
    std::optional<std::string> name;
    std::vector<std::string> data;
};

#endif

