
#ifndef JSONPARSER_UTILS_H
#define JSONPARSER_UTILS_H

#include <defines.h>
#include <NodeValue.h>


enum class NodeType {
    SIMPLE,
    OBJECT,
    ARRAY
};


class Utils
{
public:

    NodeInternal getNodeInternal(const Node&);

    NodeType getNodeType(const Node&);
    NodeType getNodeInternalType(const NodeInternal&);
};

#endif

