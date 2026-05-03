
#include <vector>

#include "token.h"
#include "../error.h"
#include "../log/ErrorStorage.h"


enum class StateAngle
{
    INITIAL,
    OPEN,
    CLOSE
};

enum class StateTag
{
    INITIAL,
    OPEN,
    CLOSE
};


static bool validate(const std::vector<TokenXML>& tokens)
{
    if (tokens.empty()) {
        return false;
    }
    if (tokens.at(0).type != TokenTypeXML::ANGLE_OPEN || tokens.at(tokens.size() - 1).type != TokenTypeXML::ANGLE_CLOSE) {
        return false;
    }

    StateAngle stateAngle = StateAngle::INITIAL;
    StateTag stateTag = StateTag::INITIAL;

    for (auto it = tokens.begin(); it != tokens.end(); it++ ) {
        if (it->type == TokenTypeXML::ANGLE_OPEN) {
            if (stateAngle == StateAngle::OPEN) {
                return false;
            }
            stateAngle = StateAngle::OPEN;
            if (std::next(it, 1)->type == TokenTypeXML::SLASH) {
                if (stateTag == StateTag::CLOSE) {
                    return false;
                }
                stateTag = StateTag::CLOSE;
            }
        }
        else if (it->type == TokenTypeXML::ANGLE_CLOSE) {
            if (stateAngle == StateAngle::CLOSE) {
                return false;
            }
        }
    }
    return true;
};


