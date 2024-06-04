
#include "Error.h"


void Error::setInfo(ErrorCode result, std::optional<std::string> info)
{
    errorCode = result;
    this->info = info;
}


ErrorCode Error::getResult()
{
    return errorCode;
}

