
#pragma once

#include <string>

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

    static void setPretendedKey(const std::string& key)
    {
        pretendedKey = key;
    }

    static std::string& getPretendedKey()
    {
        return pretendedKey;
    }

    static void setXmlRoot(bool root)
    {
        xmlRootIfNeeded = root;
    }

    static bool getXmlRoot()
    {
        return xmlRootIfNeeded;
    }

private:
    inline static unsigned int indentation = 2;
    inline static std::string pretendedKey = "__text";
    inline static bool xmlRootIfNeeded = true;
};

