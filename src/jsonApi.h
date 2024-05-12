
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../headers/defines.h"
#include <NodeValue.h>


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;


class jsonApi
{
public:

    jsonApi() = default;

    bool parseJsonString(const std::string& file);
    std::string parseObjectToJsonString(ObjectNode* node);

    ObjectNode* getRoot();

    Result getLastError();

private:

    Result result;
    std::unique_ptr<ObjectNode> root;
};

