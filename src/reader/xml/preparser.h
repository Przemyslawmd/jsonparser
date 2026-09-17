
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "token.h"


namespace xml
{
    std::unique_ptr<std::vector<Token>> parseXML(std::string_view xml);
}

