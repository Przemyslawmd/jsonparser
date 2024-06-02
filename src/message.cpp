
#include "message.h"


const std::string& Message::getMessage(Result result)
{
    return messages.at(result);
}