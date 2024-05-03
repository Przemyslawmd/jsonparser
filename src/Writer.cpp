
#include "Writer.h"

#include <variant>


std::string Writer::createJsonString(ObjectNode* object)
{
    parseObject(object);
    json = stream.str();
    return json;
}


void Writer::parseObject(ObjectNode* jsonObject)
{    
    stream << "{";
    margin += 2;
    for (auto const& [key, val] : *jsonObject) {
        stream << std::endl;
        stream.width(margin);
        stream.fill(' ');
        stream << "\"" << key << "\"" << ": ";
        if (std::holds_alternative<std::string>(val.value)) {
            stream << "\"" << std::get<std::string>(val.value) << "\"" << ",";
        }
        else if (std::holds_alternative<int>(val.value)) {
            stream << std::to_string(std::get<int>(val.value)) << ",";
        }
        else if (std::holds_alternative<double>(val.value)) {
            stream << std::to_string(std::get<double>(val.value)) << ",";
        }
        else if (std::holds_alternative<bool>(val.value)) {           
            if (std::get<bool>(val.value) == true) {
                stream << "true" << ",";
            }
            else {
                stream << "false" << ",";
            }
        }
        else if (std::holds_alternative<ObjectNode>(val.value)) {   
            const ObjectNode* obj = std::get_if<ObjectNode>(&val.value);
            parseObject((ObjectNode*) obj);
        }
    }
    stream << std::endl;
    margin -= 2;   
    stream.width(margin);
    stream.fill(' ');
    stream << "}";    
}

