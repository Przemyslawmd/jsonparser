
#pragma once

#include <ranges>
#include <vector>

#include "../../headers/elem.h"


namespace xml
{
static void checkArrays(std::vector<Elem>& elems)
{
    using enum ElemType;

    auto findOpenElem = [](std::vector<Elem>& elems, unsigned int idx, const std::string& name)
    {
        unsigned int guard = 0;
        for (auto& elem : std::views::reverse(elems) | std::views::drop(idx)) {
            if (elem.name == name && elem.type == TAG_CLOSE) {
                guard++;
            }
            else if (elem.name == name && elem.type == TAG_OPEN && guard > 0) {
                guard--;
            }
            else if (elem.name == name && elem.type == TAG_OPEN && guard == 0) {
                elem.type = TAG_ARRAY_BEGIN;
                break;
            }
        }
    };

    auto findCloseElem = [](std::vector<Elem>& elems, unsigned int idx, const std::string& name)
    {
        unsigned int guard = 0;
        for (auto& elem : elems | std::views::drop(idx)) {
            if (elem.name == name && elem.type == TAG_OPEN) {
                guard++;
            }
            else if (elem.name == name && elem.type == TAG_CLOSE && guard > 0) {
                guard--;
            }
            else if (elem.name == name && elem.type == TAG_CLOSE && guard == 0) {
                elem.type = TAG_ARRAY_END;
                break;
            }
        }
    };

    auto firstToken = elems.front().type == DECLARATION ? 1 : 0;
    std::optional<std::string> tagName;

    for (auto [idx, elem] : elems | std::views::enumerate | std::views::drop(firstToken)) {
        if (elem.type == TAG_CLOSE) {
            tagName = elem.name;
            continue;
        }
        if (elem.type == TAG_OPEN && tagName.has_value() && tagName.value() == elem.name) {
            elem.type = TAG_ARRAY_OPEN;
            elems.at(idx - 1).type = TAG_ARRAY_CLOSE;
            findOpenElem(elems, idx - 1, tagName.value());
            findCloseElem(elems, idx + 1, tagName.value());
        }
        tagName.reset();
    }
}
}
