
#ifndef JSONPARSER_NODE_H
#define JSONPARSER_NODE_H

#include <map>
#include <string>
#include <vector>
#include <variant>


struct Node
{
    using Value = std::variant<std::string, int64_t, double, bool, nullptr_t, std::map<size_t, Node>, std::vector<Node>>;
    Value value;
};


using ObjectNode = std::map<size_t, Node>;
using ArrayNode = std::vector<Node>;

#endif

