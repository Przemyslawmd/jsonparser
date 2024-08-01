
#include "Writer.h"

#include <iterator>
#include <variant>


constexpr std::string_view dataEnd = ",\n";


std::string Writer::createJsonString(ObjectNode* object)
{
    processObject(object);
    return stream.str();
}


void Writer::setMarginStep(size_t marginStep)
{
    this->marginStep = marginStep;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::processObject(const ObjectNode* jsonObject)
{
    stream << "{\n";
    incMargin();

    for (auto const& [idKey, val] : *jsonObject) {
        std::fill_n(std::ostream_iterator<char>(stream), margin, ' ');
        std::string keyStr = keyMapper.getStrKey(idKey).value();
        stream << "\"" << keyStr << "\": ";
        parseData(val);
    }
    deleteLastChars(stream);

    decMargin();
    if (margin == 0) {
        stream << '}';
        return;
    }
    std::fill_n(std::ostream_iterator<char>(stream), margin, ' ');
    stream << "},\n"; 
}


void Writer::processArray(const ArrayNode* jsonArray)
{
    stream << "[\n";
    incMargin();
    
    for (auto const& val : *jsonArray) {
        std::fill_n(std::ostream_iterator<char>(stream), margin, ' ');
        parseData(val);
    }    
    deleteLastChars(stream);

    decMargin();
    std::fill_n(std::ostream_iterator<char>(stream), margin, ' ');
    stream << "],\n";
}


void Writer::parseData(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        stream << "\"" << std::get<std::string>(node.value) << "\"" << dataEnd;
    }
    else if (std::holds_alternative<int64_t>(node.value)) {
        stream << std::get<int64_t>(node.value) << dataEnd;
    }
    else if (std::holds_alternative<double>(node.value)) {
        stream << std::get<double>(node.value) << dataEnd;
    }
    else if (std::holds_alternative<bool>(node.value)) {
        stream << (std::get<bool>(node.value) == true ? "true" : "false") << dataEnd;
    }
    else if (std::holds_alternative<nullptr_t>(node.value)) {
        stream << "null" << dataEnd;
    }
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        processObject(std::get_if<ObjectNode>(&node.value));
    }
    else if (std::holds_alternative<ArrayNode>(node.value)) {
        processArray(std::get_if<ArrayNode>(&node.value));
    }
}


void Writer::deleteLastChars(std::ostringstream& stream)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - 2);
    stream << "\n";
}


void Writer::incMargin()
{
    margin += marginStep;
}


void Writer::decMargin()
{
    margin -= marginStep;
}

