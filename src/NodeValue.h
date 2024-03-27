
#ifndef JSONCPP_NODE_H
#define JSONCPP_NODE_H

struct Node
{
    using Value = std::variant<std::string, int, double, bool, std::map<std::string, Node>>;
    Value value;
};

#endif

