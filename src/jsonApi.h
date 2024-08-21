
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
#include "utils.h"
#include "manager.h"


/* Pointer to complex node: object (map) or array */
using ComplexNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


class JsonApi
{
public:

    JsonApi();

    bool parseJsonString(const std::string& file);

    std::string parseObjectToJsonString();

    bool loadObjectJson(const Node&);

    void clear();

    bool addNodeIntoObject(const std::vector<Path>& path, const std::string& keyStr, const Node&);
    bool addNodeIntoArray(const std::vector<Path>& path, const Node&);
    bool insertNodeIntoArray(const std::vector<Path>& path, size_t index, const Node&);

    bool changeNodeInObject(const std::vector<Path>& path, const std::string& keyStr, const Node&);
    bool changeNodeInArray(const std::vector<Path>& path, size_t index, const Node&);

    bool removeNodeFromObject(const std::vector<Path>& path, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>& path, size_t index);

    ErrorCode getErrorCode();

private:

    template <typename T>
    T* putIntoObjectAndGet(ObjectNode*, uint32_t keyID);

    template <typename T>
    T* putIntoArrayAndGet(ArrayNode*);

    template <typename T>
    bool validateNodeType(ComplexNodePtr, ErrorCode potentialError);

    std::unique_ptr<Manager> manager;
    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Utils> utils;
    std::unique_ptr<Error> error;
};

#endif

