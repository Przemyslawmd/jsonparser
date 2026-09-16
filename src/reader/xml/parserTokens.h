
#pragma once

#include <memory>
#include <vector>

#include "token.h"
#include "headers/elem.h"


namespace xml
{
    std::unique_ptr<std::vector<Elem>> parseTokens(const std::vector<Token>& tokens);
}

