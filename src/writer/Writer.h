
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

    std::string createJsonString(ObjectNode*);

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
};

#endif

