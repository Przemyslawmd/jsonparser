
#ifndef JX_READER_XML_VALIDATOR_H
#define JX_READER_XML_VALIDATOR_H

#include <vector>
#include <ranges>
#include <stack>

#include "elem.h"
#include "token.h"
#include "errorCode.h"
#include "log/ErrorStorage.h"


namespace xml
{

static bool ValidateElems(std::vector<ElemReader>& elems)
{
    uint skipFirst = elems.at(0).type == ElemType::DECLARATION ? 1 : 0;

    std::stack<std::string> tags;
    for (const auto& elem : elems | std::views::drop(skipFirst)) 
    {
        if (elem.type == ElemType::DECLARATION) {
            ErrorStorage::putError(ErrorCode::XML_VALIDATOR_DECLARATION_NOT_START);
            return false;
        }
        if (elem.type == ElemType::TAG_OPEN) {
            tags.push(elem.name);
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
}

#endif

