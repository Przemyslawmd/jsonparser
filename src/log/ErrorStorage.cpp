
#include "ErrorStorage.h"


const std::vector<Error>& ErrorStorage::getErrors()
{
    return errors;
}


void ErrorStorage::putError(ErrorCode errorCode, std::optional<std::string> details)
{
    errors.emplace_back(errorCode, details);
}


void ErrorStorage::clear()
{
    errors.clear();
}


std::vector<Error> ErrorStorage::errors = {};

