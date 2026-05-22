
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
    for (size_t i = skipFirst; i < elems.size(); i++) 
    {
        if (elems[i].type == ElemType::DECLARATION) {
            ErrorStorage::putError(ErrorCode::XML_VALIDATOR_DECLARATION_NOT_START);
            return false;
        }
        if (elems[i].type == ElemType::TAG_OPEN) {
            tags.push(elems[i].name.value());
            continue;
        }
        if (elems[i].type == ElemType::TAG_CLOSE) {
            if (elems[i].name != tags.top()) {
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

