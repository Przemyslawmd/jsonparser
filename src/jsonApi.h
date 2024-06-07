
#ifndef JSONPARSER_API_H
#define JSONPARSER_API_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <defines.h>
#include <NodeValue.h>
#include <Error.h>


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;

/* Pointer to internal node: object (map) or array */
using InnerNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Indicator is a key for object (map) or index for an array */
using Indicator = std::variant<std::string, size_t>;


class JsonApi
{
public:

    JsonApi() = default;

    bool parseJsonString(const std::string& file);
    std::string parseObjectToJsonString();

    bool loadObject(std::unique_ptr<ObjectNode>);

    void clear();

    ObjectNode* getRoot();
    InnerNodePtr getNode(const std::vector<Indicator>&);

    bool changeNodeInObject(const std::vector<Indicator>&, const std::string& key, Node);
    bool changeNodeInArray(const std::vector<Indicator>&, size_t index, Node);

    bool addNodeIntoObject(const std::vector<Indicator>&, const std::string& key, Node);
    bool addNodeIntoArray(const std::vector<Indicator>&, Node);
    bool insertNodeIntoArray(const std::vector<Indicator>&, int index, Node);

    bool removeNodeFromObject(const std::vector<Indicator>&, const std::string& key);
    bool removeNodeFromArray(const std::vector<Indicator>&, size_t index);

    ErrorCode getErrorCode();

private:

    bool isRootEmpty();

    ArrayNode* getArrayAndCheckIndex(const std::vector<Indicator>&, size_t index);
    ObjectNode* getObjectAndCheckKey(const std::vector<Indicator>&, const std::string& key);

    template <typename T>
    bool validateNodeType(InnerNodePtr, ErrorCode potentialError);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<Error> error;
};

#endif

