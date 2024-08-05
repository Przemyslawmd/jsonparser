
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

    bool loadObjectJson(const Node&);

    //void clear();

    bool addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node&);
    bool addNodeIntoArray(const std::vector<Path>& path, const Node&);
    bool insertNodeIntoArray(const std::vector<Path>& path, size_t index, const Node&);

    bool changeNodeInObject(const std::vector<Path>& path, const std::string& keyStr, const Node&);
    bool changeNodeInArray(const std::vector<Path>& path, size_t index, const Node&);

    bool removeNodeFromObject(const std::vector<Path>& path, const std::string& key);
    bool removeNodeFromArray(const std::vector<Path>& path, size_t index);

    ErrorCode getErrorCode();

private:

    bool isRootEmpty();

    InnerNodePtr getNodeFromPath(const std::vector<Path>& path);

    NodeType getNodeType(const Node&);
    NodeType getNodeInternalType(const NodeInternal&);

    NodeInternal getNodeInternal(const Node&);

    ArrayNode* getArrayAndCheckIndex(const std::vector<Path>& path, size_t index);

    std::tuple<ObjectNode*, size_t>
    getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr);

    bool addObjectNodeInternally(ObjectNode*, const Node&);
    bool addArrayNodeInternally(ArrayNode*, const Node&);

    bool traverseObjectToRemoveKeyID(ObjectNode*);
    bool traverseArrayToRemoveKeyID(ArrayNode*);

    template <typename T>
    T* putIntoObjectAndGet(ObjectNode* obj, uint32_t itemID);

    template <typename T>
    T* putIntoArrayAndGet(ArrayNode* obj);

    template <typename T>
    bool validateNodeType(InnerNodePtr, ErrorCode potentialError);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Error> error;
};

#endif

