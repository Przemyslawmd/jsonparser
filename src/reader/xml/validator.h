
#include <vector>
#include <ranges>
#include <stack>

#include "elem.h"
#include "error.h"
#include "log/ErrorStorage.h"


static bool ValidateAttrs(const std::vector<TokenXML>& attrs)
{
    using enum TokenTypeXML;

    if (attrs[0].type != DATA_STR) {
        ErrorStorage::putError(ErrorCode::XML_VALIDATOR_ATTR);
        return false;
    }

    TokenTypeXML prev = DATA_STR;
    for (const auto& attr : attrs | std::views::drop(1)) {
        if ((attr.type == DATA_STR && prev != DATA_STR_QUOTA) ||
            (attr.type == EQUAL && prev != DATA_STR) ||
            (attr.type == DATA_STR_QUOTA && prev != EQUAL)) {
                ErrorStorage::putError(ErrorCode::XML_VALIDATOR_ATTR);
                return false;
        }
        prev = attr.type;
    }

    if (prev != DATA_STR_QUOTA) {
        ErrorStorage::putError(ErrorCode::XML_VALIDATOR_ATTR);
        return false;
    }
    return true;
}


static bool ValidateElems(std::vector<Elem>& elems)
{
    uint skipFirst = 0;
    if (elems.at(0).type == ElemType::DECLARATION) {
        if (!elems.at(0).attr.empty() && !ValidateAttrs(elems.at(0).attr)) {
            ErrorStorage::putError(ErrorCode::XML_VALIDATOR_ATTR);
            return false;
        }
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
            if (!elem.attr.empty() && !ValidateAttrs(elem.attr)) {
                return false;
            }
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

