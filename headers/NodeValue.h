
#ifndef JSONPARSER_NODE_H
#define JSONPARSER_NODE_H

#include <map>
#include <string>
#include <vector>
#include <variant>


struct Node
{
    using Value = std::variant<std::string, int64_t, double, bool, std::map<std::string, Node>, std::vector<Node>>;
    Value value;
};

#endif

