
#ifndef JSONPARSER_NODE_H
#define JSONPARSER_NODE_H

#include <map>
#include <string>
#include <vector>
#include <variant>


struct NodeInternal
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<size_t, NodeInternal>, std::vector<NodeInternal>>;
    Value value;
};


using ObjectNode = std::map<size_t, NodeInternal>;
using ArrayNode = std::vector<NodeInternal>;


struct NodeExternal
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<std::string, NodeExternal>, std::vector<NodeExternal>>;
    Value value;
};


using ObjectNodeExternal = std::map<std::string, NodeExternal>;
using ArrayNodeExternal = std::vector<NodeExternal>;


#endif

