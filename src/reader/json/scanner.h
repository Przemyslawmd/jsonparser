
#pragma once

#include <memory>
#include <vector>

#include "token.h"


namespace json
{
    std::unique_ptr<std::vector<Token>> scanString(std::string_view json);
}

