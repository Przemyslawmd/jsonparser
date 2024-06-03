
#include "Error.h"


void Error::setInfo(Result result, std::optional<std::string> info)
{
    errorCode = result;
    this->info = info;
}


Result Error::getResult()
{
    return errorCode;
}

