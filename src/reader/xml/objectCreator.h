
#ifndef JSONPARSER_OBJECT_CREATOR_H
#define JSONPARSER_OBJECT_CREATOR_H

#include <map>
#include <memory>
#include <stack>
#include <variant>

#include "../keyMapper.h"
#include "elem.h"
#include "node.h"
#include "state.h"
#include "token.h"


class ObjectCreator
{
    public:
        ObjectCreator(KeyMapper& keyMapper) : keyMapper(keyMapper) {};

        std::unique_ptr<ObjectNode> parseElems(std::vector<Elem>&);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::unique_ptr<ObjectNode> nodes;

        KeyMapper& keyMapper;
        std::stack<uint32_t> mapIDStack;
        std::stack<std::string> keyStack;
        uint32_t maxMapId = 0;

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack);
        void popDataFromStack();
        void processTagOpen(const std::string& key);
        void processContent(const std::vector<TokenXML>& attrs);
};

#endif

