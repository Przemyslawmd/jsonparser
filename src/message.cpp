
#include "message.h"


std::string Message::getMessage(ErrorCode result)
{
    if (messages.at(result).has_value()) {
        return messages.at(result).value();
    }
    return { "No details" };
}

