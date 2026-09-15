
#pragma once

#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include "keyMapper.h"
#include "../../headers/elem.h"
#include "node.h"
#include "settings.h"
#include "state.h"
#include "token.h"


template <typename T>
concept ComplexNodeObjectCreator = std::same_as<T, ObjectNode> || std::same_as<T, ArrayNode>;


namespace xml
{
class ObjectCreator
{
public:
    explicit ObjectCreator(KeyMapper& keyMapper) : keyMapper(keyMapper),
                                                   attrs(nullptr),
                                                   pretendedKey(Settings::getPretendedKey()),
                                                   maxMapId(0) {};

    std::unique_ptr<ObjectNode> parseElems(std::vector<Elem>&);

private:
    KeyMapper& keyMapper;

    std::stack<ObjectNode*> objStack;
    std::stack<ArrayNode*> arrStack;

    std::vector<std::tuple<std::string, std::string>>* attrs;
    const std::string& pretendedKey;

    std::stack<uint32_t> mapIDStack;
    std::stack<std::string> keyStack;
    std::stack<State> stateStack;
    uint32_t maxMapId;

    template <typename T> requires ComplexNodeObjectCreator<T>
    void pushContext(T* node, const std::string& key, State);

    void pushContext(ArrayNode*, State);
    void popContext();

    void processTagOpen(const std::string& key);
    void processTagArrayOpen(const std::string& key);
    void processContent(TokenData& data);

    void insertAttrs(ObjectNode& node, std::vector<std::tuple<std::string, std::string>>& attrs);
};
}

