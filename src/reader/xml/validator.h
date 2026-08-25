
#pragma once

#include <vector>
#include <ranges>
#include <stack>

#include "../../headers/elem.h"
#include "errorCode.h"
#include "log/ErrorStorage.h"


namespace xml
{
static bool ValidateElems(const std::vector<Elem>& elems)
{
    unsigned int skipFirst = elems.front().type == ElemType::DECLARATION ? 1 : 0;

    std::stack<std::string> tags;
    for (const auto& elem : elems | std::views::drop(skipFirst)) 
    {
        if (elem.type == ElemType::DECLARATION) {
            ErrorStorage::putError(ErrorCode::XML_VALIDATOR_DECLARATION_IMPROPER_PLACE);
            return false;
        }
        if (elem.type == ElemType::TAG_OPEN) {
            tags.push(elem.name.value());
            continue;
        }
        if (elem.type == ElemType::TAG_CLOSE) {
            if (elem.name != tags.top()) {
                ErrorStorage::putError(ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
                return false;
            }
            tags.pop();
        }
    }

    if (tags.empty()) {
        return true;
    }
    ErrorStorage::putError(ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
    return false;
}
}

