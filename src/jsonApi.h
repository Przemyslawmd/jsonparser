
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
using InnerNodePtr = std::variant<Object*, Array*, nullptr_t>;

/* Indicator is a key for object (map) or index for an array */
using Indicator = std::variant<std::string, size_t>;


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
    
    std::string parseObjectToJsonString();

    //bool loadObject(std::unique_ptr<Object>);

    //void clear();

    bool changeNodeInObject(const std::vector<Indicator>&, const std::string& key, Node);
    bool changeNodeInArray(const std::vector<Indicator>&, size_t index, Node);

    bool addNodeIntoObject(const std::vector<Indicator>&, const std::string& key, Node);
    bool addNodeIntoArray(const std::vector<Indicator>&, Node);
    bool insertNodeIntoArray(const std::vector<Indicator>&, int index, Node);

    //bool removeNodeFromObject(const std::vector<Indicator>&, const std::string& key);
    //bool removeNodeFromArray(const std::vector<Indicator>&, size_t index);

    ErrorCode getErrorCode();

private:

    bool isRootEmpty();

    InnerNodePtr getNode(const std::vector<Indicator>& path);

    NodeType getNodeType(Node& node);

    NodeInternal getNodeInternalFromNode(Node&);

    Array* getArrayAndCheckIndex(const std::vector<Indicator>& path, size_t index);
    Object* getObjectAndCheckKey(const std::vector<Indicator>& path, const std::string& key);

    bool addObjectInternally(Object*, Node);
    bool addArrayInternally(Array*, Node);

    template <typename T>
    bool validateNodeType(InnerNodePtr, ErrorCode potentialError);

    std::unique_ptr<Object> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Error> error;
};

#endif

