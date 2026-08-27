
#include "parser.h"

#include <ranges>


using namespace json;
using enum TokenType;
using enum State;

std::unique_ptr<ObjectNode> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    auto nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushDataOnStack(nodes.get(), OBJECT_PARSING);

    for (const auto& [type, data] : tokens | std::views::take(tokens.size() - 1) | std::views::drop(1)) {
        bool result = true;
        switch (type) {
            case CURLY_CLOSE:
            case SQUARE_CLOSE:
                popDataFromStack();
                break;
            case KEY:
                key = std::get<std::string>(data);
                break;
            case DATA_INT:
                result = processData(key, std::get<int64_t>(data));
                break;
            case DATA_DOUBLE:
                result = processData(key, std::get<double>(data));
                break;
            case DATA_STR:
                result = processData(key, std::get<std::string>(data));
                break;
            case DATA_BOOL:
                result = processData(key, std::get<bool>(data));
                break;
            case DATA_NULL:
                result = processData(key, std::get<nullptr_t>(data));
                break;
            case CURLY_OPEN:
                result = pushNodeOnStack<ObjectNode>(key, OBJECT_PARSING);
                break;
            case SQUARE_OPEN:
                result = pushNodeOnStack<ArrayNode>(key, ARRAY_PARSING);
                break;
        }
        if (result == false) {
            return nullptr;
        }
    }
    return nodes;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

template <typename T> requires ComplexNode<T>
bool Parser::pushNodeOnStack(const std::string& key, const State state)
{
    if (stateStack.top() == OBJECT_PARSING) {
        auto optKeyID = keyMapper.createKeyID(key, mapIDStack.top());
        if (!optKeyID.has_value()) {
            return false;
        }

        ObjectNode* objectNode = std::get<ObjectNode*>(nodeStack.top());
        const auto [it, _] = objectNode->emplace(optKeyID.value(), T());
        pushDataOnStack(&std::get<T>((it->second).value), state);
    }
    else {
        ArrayNode* arrayNode = std::get<ArrayNode*>(nodeStack.top());
        auto& ref = arrayNode->emplace_back(T());
        pushDataOnStack(&std::get<T>(ref.value), state);
    }
    return true;
}


template <typename T> requires SimpleNode<T>
bool Parser::processData(const std::string& key, const T& data)
{
    if (stateStack.top() == OBJECT_PARSING) {
        auto optKeyID = keyMapper.createKeyID(key, mapIDStack.top());
        if (!optKeyID.has_value()) {
            return false;
        }
        std::get<ObjectNode*>(nodeStack.top())->emplace(optKeyID.value(), data);
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(data);
    }
    return true;
}


void Parser::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
    if (state == OBJECT_PARSING) {
        maxMapId += (1 << 16);
        mapIDStack.push(maxMapId);
    }
}


void Parser::popDataFromStack()
{
    if (stateStack.top() == OBJECT_PARSING) {
        mapIDStack.pop();
    }
    nodeStack.pop();
    stateStack.pop();
}

