
#include "Parser.h"

#include <stack>


std::unique_ptr<ObjectNode> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    bool result;
    auto nodes = std::make_unique<ObjectNode>();
    mapIDStack.push(0);
    pushDataOnStack(nodes.get(), State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {
        result = true;
        if (it->type == TokenType::CURLY_CLOSE || it->type == TokenType::SQUARE_CLOSE) {
            popDataFromStack();
        }
        else if (it->type == TokenType::KEY) {
            key = std::get<std::string>(it->data);
        }
        else if (it->type == TokenType::DATA_INT) {
            result = processData<int64_t>(key, *it);
        }
        else if (it->type == TokenType::DATA_DOUBLE) {
            result = processData<double>(key, *it);
        }
        else if (it->type == TokenType::DATA_STR) {
            result = processData<std::string>(key, *it);
        }
        else if (it->type == TokenType::DATA_BOOL) {
            result = processData<bool>(key, *it);
        }
        else if (it->type == TokenType::DATA_NULL) {
            result = processData<nullptr_t>(key, *it);
        }
        else if (it->type == TokenType::CURLY_OPEN) {
            result = pushInnerNodeOnStack<ObjectNode>(key, State::OBJECT_PARSING);
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            pushInnerNodeOnStack<ArrayNode>(key, State::ARRAY_PARSING);
        }

        if (result == false) {
            return nullptr;
        }
    }
    return nodes;
}


std::unique_ptr<Error> Parser::getError()
{
    return std::move(error);
}

/*******************************************************************/
/* PRIVATE *********************************************************/

template <typename T>
bool Parser::pushInnerNodeOnStack(const std::string& keyStr, State state)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        ObjectNode* obj = std::get<ObjectNode*>(nodeStack.top());

        auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        if (optKeyID == std::nullopt) {
            error = keyMapper.getError();
            return false;
        }
        uint32_t keyID = optKeyID.value();

        obj->emplace(std::make_pair(keyID, T()));
        auto* currentNode = &(std::get<T>(obj->at(keyID).value));
        pushDataOnStack(currentNode, state);
    }
    else {
        ArrayNode* arr = std::get<ArrayNode*>(nodeStack.top());
        arr->emplace_back(T());
        auto* currentNode = &(std::get<T>(arr->back().value));
        pushDataOnStack(currentNode, state);
    }
    return true;
}


template <typename T>
bool Parser::processData(const std::string& keyStr, const Token& token)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        auto optKeyID = keyMapper.createAndPutKeyID(keyStr, mapIDStack.top());
        if (optKeyID == std::nullopt) {
            error = keyMapper.getError();
            return false;
        }
        uint32_t keyID = optKeyID.value();
        std::get<ObjectNode*>(nodeStack.top())->emplace(std::make_pair(keyID, std::get<T>(token.data)));
    }
    else {
        std::get<ArrayNode*>(nodeStack.top())->emplace_back(std::get<T>(token.data));
    }
    return true;
}


void Parser::pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
    if (state == State::OBJECT_PARSING) {
        maxMapId += (1 << 16);
        mapIDStack.push(maxMapId);
    }
}


void Parser::popDataFromStack()
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        mapIDStack.pop();
    }
    nodeStack.pop();
    stateStack.pop();
}

