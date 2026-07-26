
#ifndef JX_MANAGER_H
#define JX_MANAGER_H

#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "keyMapper.h"
#include "node.h"
#include "nodeApi.h"
#include "errorCode.h"
#include "settings.h"


using ComplexNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, uint>;


class Manager
{
public:
    Manager();

    bool parseJsonString(const std::string& file);
    bool parseXmlString(const std::string& file);

    std::optional<std::string> objectToJsonString();
    std::optional<std::string> objectToXmlString();

    bool loadObject(const NodeApi&);
    bool isObject() const;

    void clear();

    bool addNodeIntoObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool addNodeIntoArray(const std::vector<Path>&, const NodeApi&);
    bool insertNodeIntoArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool changeNodeInObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool changeNodeInArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool removeNodeFromObject(const std::vector<Path>&, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>&, size_t index);

    bool setIndentation(uint indentation);

    const std::vector<Error>& getErrors() const;

private:
    bool isRootEmpty() const;

    void addObjectInternally(ObjectNode&, const NodeApi&);
    void addArrayInternally(ArrayNode&, const NodeApi&);

    ComplexNodePtr getNodeFromPath(const std::vector<Path>& path);

    ArrayNode* getArrayFromPath(const std::vector<Path>& path, size_t index);

    std::tuple<ObjectNode*, size_t> 
    getObjectAndKeyIDFromPath(const std::vector<Path>& path, const std::string& keyStr);

    void traverseObjectToRemoveKeyID(const ObjectNode&);
    void traverseArrayToRemoveKeyID(const ArrayNode&);
    void checkObjectToRemoveKeyID(const Node&);

    std::unique_ptr<ObjectNode> root;
    std::unique_ptr<KeyMapper> keyMapper;
};

#endif
