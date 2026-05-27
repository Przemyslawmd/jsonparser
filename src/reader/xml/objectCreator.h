
#ifndef JSONPARSER_OBJECT_CREATOR_H
#define JSONPARSER_OBJECT_CREATOR_H

#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include "../keyMapper.h"
#include "elem.h"
#include "node.h"
#include "state.h"
#include "token.h"


class ObjectCreator
{
    public:
        ObjectCreator() = default;

        std::unique_ptr<ObjectNode> parseElems(std::vector<Elem>&);

    private:
        std::stack<std::variant<ObjectNode*, ArrayNode*>> nodeStack;
        std::unique_ptr<ObjectNode> nodes;

        void pushDataOnStack(std::variant<ObjectNode*, ArrayNode*> nodeStack);
        void popDataFromStack();
        void processTagOpen(uint key);
        void processContent(uint key);
};

#endif

