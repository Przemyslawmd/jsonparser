
#ifndef JSONPARSER_NODE_API_H
#define JSONPARSER_NODE_API_H

#include <map>
#include <string>
#include <variant>
#include <vector>


struct NodeApi
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<std::string, NodeApi>, std::vector<NodeApi>>;
    Value value;
};

#endif

