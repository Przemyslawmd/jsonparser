
#include <string>
#include <variant>

#include <defines.h>


class Token
{
public:
   
    TokenType type;
    std::variant<std::string, int, float, bool> data;
};

