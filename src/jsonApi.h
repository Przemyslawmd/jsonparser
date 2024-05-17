
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


class jsonApi
{
public:

    jsonApi() = default;

    bool parseJsonString(const std::string& file);
    std::string parseObjectToJsonString(ObjectNode* node);

    ObjectNode* getRoot();

    std::string getNodeType(const std::vector<Indicator>& keys);
    InnerNodePtr getNode(const std::vector<Indicator>& keys);
    bool changeNodeValue(const std::vector<Indicator>& keys, Node node);

    Result getLastError();

private:

    Result result;
    std::unique_ptr<ObjectNode> root;
};

