
#include "Parser.h"

#include <stack>


constexpr size_t BITS_16 = 16;


std::unique_ptr<Object> Parser::parseTokens(const std::vector<Token>& tokens)
{
    std::string key;
    auto nodes = std::make_unique<Object>();
    idStack.push({ 0, 0 });
    pushDataOnStack(nodes.get(), State::OBJECT_PARSING);

    for (auto it = tokens.begin() + 1; it != tokens.end() - 1; it++) {
        if (it->type == TokenType::CURLY_CLOSE || it->type == TokenType::SQUARE_CLOSE) {
            popDataFromStack();
        }
        else if (it->type == TokenType::KEY) {
            key = std::get<std::string>(it->data);
        }
        else if (it->type == TokenType::DATA_INT) {
            processData<int64_t>(key, *it);
        }
        else if (it->type == TokenType::DATA_DOUBLE) {
            processData<double>(key, *it);
        }
        else if (it->type == TokenType::DATA_STR) {
            processData<std::string>(key, *it);
        }
        else if (it->type == TokenType::DATA_BOOL) {
            processData<bool>(key, *it);
        }
        else if (it->type == TokenType::DATA_NULL) {
            processData<nullptr_t>(key, *it);
        }
        else if (it->type == TokenType::CURLY_OPEN) {
            pushInnerNodeOnStack<Object>(key, State::OBJECT_PARSING);
        }
        else if (it->type == TokenType::SQUARE_OPEN) {
            pushInnerNodeOnStack<Array>(key, State::ARRAY_PARSING);
        }
    }
    return nodes;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

template <typename T>
void Parser::pushInnerNodeOnStack(const std::string& key, State state)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        Object* obj = std::get<Object*>(nodeStack.top());
        size_t id = createId();
        keyMapper.putKey(key, id);
        obj->emplace(std::make_pair(id, T()));
        auto* currentNode = &(std::get<T>(obj->at(id).value));
        pushDataOnStack(currentNode, state);
    }
    else {
        Array* arr = std::get<Array*>(nodeStack.top());
        arr->emplace_back(T());
        auto* currentNode = &(std::get<T>(arr->back().value));
        pushDataOnStack(currentNode, state);
    }
}


template <typename T>
void Parser::processData(const std::string& key, const Token& token)
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        size_t id = createId();
        keyMapper.putKey(key, id);
        std::get<Object*>(nodeStack.top())->emplace(std::make_pair(id, std::get<T>(token.data)));
    }
    else {
        std::get<Array*>(nodeStack.top())->emplace_back(std::get<T>(token.data));
    }
}


void Parser::pushDataOnStack(std::variant<Object*, Array*> node, State state)
{
    nodeStack.push(node);
    stateStack.push(state);
    if (state == State::OBJECT_PARSING) {
        maxMapId += (1 << 16);
        idStack.push({ maxMapId, 1 });
    }
}


void Parser::popDataFromStack()
{
    if (stateStack.top() == State::OBJECT_PARSING) {
        idStack.pop();
    }
    nodeStack.pop();
    stateStack.pop();
}


size_t Parser::createId()
{
    const size_t BIT_MASK = 0b11111111111111110000000000000000;
    size_t id = (idStack.top().map & BIT_MASK) + idStack.top().node;
    idStack.top().node += 1;
    return id;
}

