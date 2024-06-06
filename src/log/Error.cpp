
#include "Error.h"

#include <format>

#include "ErrorDescription.h"


void Error::setInfo(ErrorCode errorCode, std::optional<std::string> details)
{
    this->errorCode = errorCode;
    this->details = details;
}


ErrorCode Error::getErrorCode()
{
    return errorCode;
}


std::string Error::getErrorDetails()
{
    std::optional<std::string> message = ErrorDescription::description.at(errorCode);
    return std::format("{} : {}", message == std::nullopt ? "No description for this error" : message.value(), 
                                  details == std::nullopt ? "No details for this error" : details.value());
}

