
#ifndef JSONPARSER_WRITER_H
#define JSONPARSER_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include <NodeValue.h>
#include "../keyMapper.h"


using ObjectNode = std::map<size_t, Node>;
using ArrayNode = std::vector<Node>;


class Writer
{
public:

    Writer() = default;

    std::string createJsonString(ObjectNode*, KeyMapper*);

    void setMarginStep(size_t);

private:

    void processObject(const ObjectNode*);
    void processArray(const ArrayNode*);
    void parseData(const Node&);

    void incMargin();
    void decMargin();

    void deleteLastChars(std::ostringstream& stream);

    std::ostringstream stream;

    size_t margin = 0;
    size_t marginStep = 2;
    KeyMapper* keyMapper;
};

#endif

