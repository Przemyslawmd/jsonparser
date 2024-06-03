
#include <optional>
#include <string>

#include "../headers/defines.h"

class Error
{
public:
    Error() = default;

    void setInfo(Result result, std::optional<std::string> info = std::nullopt);
    Result getResult();

private:
    std::optional<std::string> info;
    Result errorCode; 
};

