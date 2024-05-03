
#include "Writer.h"

#include <variant>


std::string Writer::createJsonString(ObjectNode* object)
{
    processObject(object);
    return stream.str();
}


void Writer::processObject(const ObjectNode* jsonObject)
{    
    stream << "{";
    marginIncrease();
    for (auto const& [key, val] : *jsonObject) {
        stream << std::endl;
        stream.width(margin);
        stream.fill(' ');
        stream << "\"" << key << "\"" << ": ";
        parseData(val);
    }
    if (stream.str().back() == ',') {
        size_t pos = stream.tellp();
        stream.seekp(pos - 1);
    }
    stream << std::endl;
    marginDecrease();
    stream.width(margin);
    stream.fill(' ');
    stream << "}";    
}


void Writer::processArray(const ArrayNode* jsonArray)
{
    stream << "[";
    for (auto const& val : *jsonArray) {
        parseData(val);        
    }
    if (stream.str().back() == ',') {
        size_t pos = stream.tellp();
        stream.seekp(pos - 1);
    }
    stream << "]";
}


void Writer::parseData(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        stream << "\"" << std::get<std::string>(node.value) << "\"" << ",";
    }
    else if (std::holds_alternative<int>(node.value)) {
        stream << std::get<int>(node.value) << ",";
    }
    else if (std::holds_alternative<double>(node.value)) {
        stream << std::get<double>(node.value) << ",";
    }
    else if (std::holds_alternative<bool>(node.value)) {
        if (std::get<bool>(node.value) == true) {
            stream << "true" << ",";
        }
        else {
            stream << "false" << ",";
        }        
    }
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        const ObjectNode* jsonObject = std::get_if<ObjectNode>(&node.value);
        processObject(jsonObject);
    }
    else if (std::holds_alternative<ArrayNode>(node.value)) {
        const ArrayNode* jsonArray = std::get_if<ArrayNode>(&node.value);
        processArray(jsonArray);
    }
}


void Writer::marginIncrease()
{
    margin += 2;
}


void Writer::marginDecrease()
{
    margin -= 2;
}

