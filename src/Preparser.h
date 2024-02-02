
#include <memory>
#include <vector>

#include <defines.h>
#include "Token.h"


class Preparser
{
public:

    Preparser();
    std::unique_ptr<std::vector<Token>> parseJSON(const std::string& json);

private:

    void parseNumber();
    void parseString();

    std::unique_ptr<std::vector<Token>> tokens;
};

