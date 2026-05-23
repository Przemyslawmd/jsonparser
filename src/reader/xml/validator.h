
#include <vector>
#include <ranges>
#include <stack>

#include "item.h"
#include "error.h"
#include "log/ErrorStorage.h"


static bool ValidateElems(std::vector<Elem>& elems)
{
    uint skipFirst = 0;
    if (elems.at(0).type == ElemType::DECLARATION) {
        skipFirst = 1;
    }

    std::stack<std::string> tags;
    for (const auto& elem : elems | std::views::drop(skipFirst)) 
    {
        if (elem.type == ElemType::DECLARATION) {
            ErrorStorage::putError(ErrorCode::XML_VALIDATOR_DECLARATION_NOT_START);
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
            continue;
        }
    }

    if (tags.empty()) {
        return true;
    }
    ErrorStorage::putError(ErrorCode::XML_VALIDATOR_MISMATCHED_TAG);
    return false;
}


static bool ValidateAttributes(Elem& elem)
{
    return false;
}
