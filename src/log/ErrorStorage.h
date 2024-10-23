
#ifndef JSONPARSER_ERROR_STORAGE_H
#define JSONPARSER_ERROR_STORAGE_H

#include <optional>
#include <string>
#include <vector>

#include "error.h"


class ErrorStorage
{
public:
    static const std::vector<Error>& getErrors();
    static void putError(ErrorCode, std::optional<std::string> details = std::nullopt);

    static void clear();

private:
    static std::vector<Error> errors;
};

#endif

