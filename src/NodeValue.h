
#ifndef JSONPARSER_NODE_H
#define JSONPARSER_NODE_H

struct Node
{
    using Value = std::variant<std::string, int, double, bool, std::map<std::string, Node>, std::vector<Node>>;
    Value value;
};

#endif

