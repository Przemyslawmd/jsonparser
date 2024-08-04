
#ifndef JSONPARSER_WRITER_H
#define JSONPARSER_WRITER_H

#include <memory>
#include <sstream> 
#include <string>
#include <variant>

#include <defines.h>
#include <NodeValue.h>
#include "../keyMapper.h"


class Writer
{
public:

    Writer(const KeyMapper& keyMapper) : keyMapper(keyMapper) {}

    std::string createJsonString(const ObjectNode*);

    void setMarginStep(size_t);

private:

    void processObjectNode(const ObjectNode*);
    void processArrayNode(const ArrayNode*);
    void parseData(const NodeInternal&);

    void incMargin();
    void decMargin();

    void deleteLastChars(std::ostringstream& stream);

    const KeyMapper& keyMapper;

    std::ostringstream stream;
    size_t margin = 0;
    size_t marginStep = 2;
};

#endif

