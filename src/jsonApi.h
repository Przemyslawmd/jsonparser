
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

/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


enum class NodeType {
    SIMPLE,
    OBJECT,
    ARRAY
};


class JsonApi
{
public:

    JsonApi();

    bool parseJsonString(const std::string& file);
    
    std::string parseObjectNodeToJsonString();

    //bool loadObjectNode(std::unique_ptr<ObjectNode>);

    //void clear();

    bool changeNodeInObject(const std::vector<Path>& path, const std::string& keyStr, Node);
    bool changeNodeInArray(const std::vector<Path>& path, size_t index, Node);

    bool addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, Node);
    bool addNodeIntoArray(const std::vector<Path>& path, Node);
    bool insertNodeIntoArray(const std::vector<Path>& path, size_t index, Node);

    bool removeNodeFromObject(const std::vector<Path>& path, const std::string& key);
    bool removeNodeFromArray(const std::vector<Path>& path, size_t index);

    ErrorCode getErrorCode();

private:

    bool isRootEmpty();

    InnerNodePtr getNode(const std::vector<Path>& path);

    NodeType getNodeType(const Node&);
    NodeType getNodeInternalType(const NodeInternal&);

    NodeInternal getNodeInternal(Node&);

    ArrayNode* getArrayNodeAndCheckIndex(const std::vector<Path>& path, size_t index);

    std::tuple<ObjectNode*, size_t>
    getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr);

    bool addObjectNodeInternally(ObjectNode*, Node);
    bool addArrayNodeInternally(ArrayNode*, Node);

    bool traverseObjectToRemoveKeyID(ObjectNode*);
    bool traverseArrayToRemoveKeyID(ArrayNode*);

    template <typename T>
    bool validateNodeType(InnerNodePtr, ErrorCode potentialError);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Error> error;
};

#endif

