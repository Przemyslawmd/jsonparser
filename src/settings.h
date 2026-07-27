
#pragma once

#include "errorCode.h"
#include "log/ErrorStorage.h"


class Settings
{
public:
    static bool setIndendation(unsigned int value) 
    {
        if (value > 10) {
            ErrorStorage::putError(ErrorCode::SETTINGS_MAX_INDENTATION);
            return false;
        }
        indentation = value;
        return true;
    }

    static unsigned int getIndentation()
    {
        return indentation;
    }

private:
    inline static unsigned int indentation = 2;
};

