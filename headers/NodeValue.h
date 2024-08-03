
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


using Object = std::map<size_t, NodeInternal>;
using Array = std::vector<NodeInternal>;


struct Node
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<std::string, Node>, std::vector<Node>>;
    Value value;
};


using ObjectExternal = std::map<std::string, Node>;
using ArrayExternal = std::vector<Node>;


#endif

