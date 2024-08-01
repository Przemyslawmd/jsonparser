
#ifndef JSONPARSER_API_H
#define JSONPARSER_API_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <defines.h>
#include "keyMapper.h"
#include <NodeValue.h>
#include <Error.h>


/* Pointer to internal node: object (map) or array */
using InnerNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;


class JsonApi
{
public:

    JsonApi();

    bool parseJsonString(const std::string& file);
    
    std::string parseObjectToJsonString();
    
    //bool loadObject(std::unique_ptr<ObjectNode>);

    //void clear();

    //ObjectNode* getRoot();

    //bool changeNodeInObject(const std::vector<Indicator>&, const std::string& key, Node);
    //bool changeNodeInArray(const std::vector<Indicator>&, size_t index, Node);

    //bool addNodeIntoObject(const std::vector<Indicator>&, const std::string& key, Node);
    //bool addNodeIntoArray(const std::vector<Indicator>&, Node);
    //bool insertNodeIntoArray(const std::vector<Indicator>&, int index, Node);

    //bool removeNodeFromObject(const std::vector<Indicator>&, const std::string& key);
    //bool removeNodeFromArray(const std::vector<Indicator>&, size_t index);

    ErrorCode getErrorCode();

private:

    bool isRootEmpty();

    InnerNodePtr getNode(const std::vector<size_t>& );

    ArrayNode* getArrayAndCheckIndex(const std::vector<size_t>&, size_t index);
    ObjectNode* getObjectAndCheckKey(const std::vector<size_t>&, const size_t key);

    template <typename T>
    bool validateNodeType(InnerNodePtr, ErrorCode potentialError);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Error> error;
};

#endif

