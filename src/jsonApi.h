
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <defines.h>
#include <NodeValue.h>


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;

/* Pointer to internal node: map or array */
using InnerNodePtr = std::variant<ObjectNode*, ArrayNode*, nullptr_t>;

/* Indicator is a key for map or index for an array */
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
    InnerNodePtr getNode(const std::vector<Indicator>& keys);

    bool changeNodeInObject(const std::vector<Indicator>& keys, const std::string& key, Node node);
    bool changeNodeInArray(const std::vector<Indicator>& keys, int index, Node node);

    bool addNodeIntoObject(const std::vector<Indicator>& keys, const std::string& key, Node node);
    bool addNodeIntoArray(const std::vector<Indicator>& keys, Node node);
    bool insertNodeIntoArray(const std::vector<Indicator>& keys, int index, Node node);

    bool removeNodeFromObject(const std::vector<Indicator>& keys, const std::string& key);
    bool removeNodeFromArray(const std::vector<Indicator>& keys, int index);

    Result getLastError();

private:

    bool isRootEmpty();

    ArrayNode* getArrayAndCheckIndex(const std::vector<Indicator>& keys, int index);
    ObjectNode* getObjectAndCheckKey(const std::vector<Indicator>& keys, const std::string& key);

    template <typename T>
    bool validateNodeType(InnerNodePtr, Result potentialError);

    Result result = Result::OK;
    std::unique_ptr<ObjectNode> root;
};

