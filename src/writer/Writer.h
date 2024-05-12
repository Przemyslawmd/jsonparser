
#ifndef JSONPARSER_WRITER_H
#define JSONPARSER_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include <NodeValue.h>


using ObjectNode = std::map<std::string, Node>;
using ArrayNode = std::vector<Node>;


class Writer
{
public:

    Writer() = default;

    std::string createJsonString(ObjectNode* jsonObject);

private:

    void processObject(const ObjectNode* jsonObject);
    void processArray(const ArrayNode* jsonArray);
    void parseData(const Node& node);

    void incMargin();
    void decMargin();
    
    std::ostringstream stream;
    size_t margin = 0;
};

#endif

