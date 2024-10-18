
#ifndef JSONPARSER_MANAGER_H
#define JSONPARSER_MANAGER_H

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "defines.h"
#include "keyMapper.h"
#include "Node.h"
#include "NodeApi.h"
#include "Error.h"


using ComplexNode = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


class Manager
{
public:
    Manager();

    bool parseJsonString(const std::string& file);
    std::optional<std::string> parseObjectToString();

    bool loadJsonObject(const NodeApi&);
    bool isJsonObject() const;

    void clear();

    bool addNodeIntoObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool addNodeIntoArray(const std::vector<Path>&, const NodeApi&);
    bool insertNodeIntoArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool changeNodeInObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool changeNodeInArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool removeNodeFromObject(const std::vector<Path>&, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>&, size_t index);

    const std::vector<Error>& getErrors() const;

private:
    bool isRootEmpty() const;

    void addObjectInternally(ObjectNode&, const NodeApi&);
    void addArrayInternally(ArrayNode&, const NodeApi&);

    ComplexNode getNodeFromPath(const std::vector<Path>& path);

    ArrayNode* 
    getArrayFromPathAndCheckIndex(const std::vector<Path>& path, size_t index);

    std::tuple<ObjectNode*, size_t> 
    getObjectFromPathAndKeyID(const std::vector<Path>& path, const std::string& keyStr);

    void traverseObjectToRemoveKeyID(const ObjectNode&);
    void traverseArrayToRemoveKeyID(const ArrayNode&);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
};

#endif
