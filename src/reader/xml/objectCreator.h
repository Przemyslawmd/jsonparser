
#ifndef JX_READER_XML_OBJECT_CREATOR_H
#define JX_READER_XML_OBJECT_CREATOR_H

#include <map>
#include <memory>
#include <stack>
#include <variant>
#include <vector>

#include "keyMapper.h"
#include "elem.h"
#include "node.h"
#include "token.h"


using NodePtr = std::variant<ObjectNode*, ArrayNode*>;

namespace xml
{
class ObjectCreator
{
public:
    ObjectCreator(KeyMapper& keyMapper) : keyMapper(keyMapper), maxMapId(0), attrs(nullptr) {};

    std::unique_ptr<ObjectNode> parseElems(std::vector<ElemReader>&);

private:
    std::stack<NodePtr> nodeStack;
    std::vector<xml::Token>* attrs;
    std::map<std::string, std::string>* attrs_;

    KeyMapper& keyMapper;
    std::stack<uint32_t> mapIDStack;
    std::stack<std::string> keyStack;
    uint32_t maxMapId;

    void pushContext(NodePtr node, const std::string& keyStr);
    void popContext();

    void processTagOpen(const std::string& key);
    void processContent(const std::string& contentName, VariantData& data);

    void insertAttrs(ObjectNode& node, std::vector<xml::Token>& attrs);
    void insertAttrs_(ObjectNode& node, std::map<std::string, std::string>& attrs);
};
}

#endif

