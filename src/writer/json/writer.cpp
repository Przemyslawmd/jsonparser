
#include "writer.h"

#include <iterator>
#include <optional>
#include <variant>


constexpr std::string_view DATA_END = ",\n";

using namespace json;


std::string Writer::createJsonString(const ObjectNode& object)
{
    processObjectNode(object);
    return stream.str();
}


void Writer::setIndent(unsigned int step)
{
    indentationStep = step;
}


/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::processObjectNode(const ObjectNode& obj)
{
    stream << "{\n";
    incIndent();

    for (auto const& [idKey, val] : obj) {
        std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
        auto keyStr = keyMapper.getKeyStr(idKey);
        stream << "\"" << keyStr.value() << "\": ";
        parseData(val);
    }
    deleteLastChars(stream);

    decIndent();
    if (indentation == 0) {
        stream << '}';
        return;
    }
    std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
    stream << "},\n"; 
}


void Writer::processArrayNode(const ArrayNode& arr)
{
    stream << "[\n";
    incIndent();

    for (const auto& val : arr) {
        std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
        parseData(val);
    }
    deleteLastChars(stream);

    decIndent();
    std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
    stream << "],\n";
}


void Writer::parseData(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        stream << "\"" << std::get<std::string>(node.value) << "\"" << DATA_END;
    }
    else if (std::holds_alternative<int64_t>(node.value)) {
        stream << std::get<int64_t>(node.value) << DATA_END;
    }
    else if (std::holds_alternative<double>(node.value)) {
        stream << std::get<double>(node.value) << DATA_END;
    }
    else if (std::holds_alternative<bool>(node.value)) {
        stream << (std::get<bool>(node.value) == true ? "true" : "false") << DATA_END;
    }
    else if (std::holds_alternative<std::nullptr_t>(node.value)) {
        stream << "null" << DATA_END;
    }
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        processObjectNode(std::get<ObjectNode>(node.value));
    }
    else if (std::holds_alternative<ArrayNode>(node.value)) {
        processArrayNode(std::get<ArrayNode>(node.value));
    }
}


void Writer::deleteLastChars(std::ostringstream& stream)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - 2);
    stream << "\n";
}


void Writer::incIndent()
{
    indentation += indentationStep;
}


void Writer::decIndent()
{
    indentation -= indentationStep;
}

