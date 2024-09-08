
#ifndef JSONPARSER_MANAGER_H
#define JSONPARSER_MANAGER_H

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <defines.h>
#include "keyMapper.h"
#include <NodeValue.h>
#include <Error.h>


/* Pointer to complex node: object (map) or array */
using ComplexNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


class Manager
{
public:
    Manager();
    
    bool parseJsonString(const std::string& file);
    std::optional<std::string> parseObjectToString();

    bool loadJsonObject(const Node&);

    void clear();

    bool addNodeIntoObject(const std::vector<Path>&, const std::string& keyStr, const Node&);
    bool addNodeIntoArray(const std::vector<Path>&, const Node&);
    bool insertNodeIntoArray(const std::vector<Path>&, size_t index, const Node&);

    bool changeNodeInObject(const std::vector<Path>&, const std::string& keyStr, const Node&);
    bool changeNodeInArray(const std::vector<Path>&, size_t index, const Node&);

    bool removeNodeFromObject(const std::vector<Path>&, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>&, size_t index);

    std::unique_ptr<Error> getError();

private:
    bool isRootEmpty();

    bool addObjectInternally(ObjectNode*, const Node&);
    bool addArrayInternally(ArrayNode*, const Node&);

    ArrayNode* getArrayAndCheckIndex(const std::vector<Path>& path, size_t index);

    std::tuple<ObjectNode*, size_t>
    getObjectAndKeyID(const std::vector<Path>& path, const std::string& keyStr);

    template <typename T>
    T* putIntoObjectAndGet(ObjectNode*, uint32_t keyID);

    template <typename T>
    T* putIntoArrayAndGet(ArrayNode*);

    void traverseObjectToRemoveKeyID(const ObjectNode&);
    void traverseArrayToRemoveKeyID(const ArrayNode&);

    ComplexNodePtr getNodeFromPath(const std::vector<Path>& path);

    template <typename T>
    bool validateComplexNode(ComplexNodePtr);
    
    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
    std::unique_ptr<Error> error;
};

#endif
