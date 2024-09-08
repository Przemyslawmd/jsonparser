
#include <Error.h>

#include <format>

#include "ErrorDetails.h"


Error::Error(ErrorCode errorCode, std::optional<std::string> details) : errorCode(errorCode), details(details) {}


ErrorCode Error::getErrorCode() const
{
    return errorCode;
}


std::string Error::getErrorDetails() const
{
    if (ErrorDetails.contains(errorCode) == false) {
        return "Exception: no key for error description";
    }
    std::optional<std::string> message = ErrorDetails.at(errorCode);
    return std::format("{} : {}", message == std::nullopt ? "No description for this error" : message.value(), 
                                  details == std::nullopt ? "No details for this error" : details.value());
}

