
#include "Error.h"

#include <format>

#include "ErrorMessage.h"


void Error::setInfo(ErrorCode errorCode, std::optional<std::string> info)
{
    this->errorCode = errorCode;
    this->info = info;
}


ErrorCode Error::getErrorCode()
{
    return errorCode;
}


std::string Error::getErrorDetails()
{
    std::optional<std::string> message = ErrorMessage::messages.at(errorCode);
    return std::format("{} : {}", message == std::nullopt ? "No error message for this error code" : message.value(), 
                                  info == std::nullopt ? "No error details for this error" : info.value());
}

