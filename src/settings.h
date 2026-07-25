
#ifndef JX_SETTINGS_H
#define JX_SETTINGS_H


class Settings
{
public:
    static bool setIndendation(uint value) 
    {
        if (value > 10) {
            return false;
        }
        indentation = value;
        return true;
    }

    static uint getIndentation()
    {
        return indentation;
    }

private:
    inline static uint indentation = 2;
};

#endif

