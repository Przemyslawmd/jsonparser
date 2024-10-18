
#include "Writer.h"

#include <iterator>
#include <optional>
#include <variant>

#include "../log/ErrorStorage.h"


constexpr std::string_view DATA_END = ",\n";


std::string Writer::createJsonString(const ObjectNode& object)
{
    processObjectNode(object);
    return stream.str();
}


void Writer::setIndent(size_t indentStep)
{
    this->indentStep = indentStep;
}


/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::processObjectNode(const ObjectNode& obj)
{
    stream << "{\n";
    incIndent();

    for (auto const& [idKey, val] : obj) {
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        auto keyStr = keyMapper.getKeyStr(idKey);
        stream << "\"" << keyStr.value() << "\": ";
        parseData(val);
    }
    deleteLastChars(stream);

    decIndent();
    if (indent == 0) {
        stream << '}';
        return;
    }
    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
    stream << "},\n"; 
}


void Writer::processArrayNode(const ArrayNode& arr)
{
    stream << "[\n";
    incIndent();
    
    for (auto const& val : arr) {
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        parseData(val);
    }
    deleteLastChars(stream);

    decIndent();
    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
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
    else if (std::holds_alternative<nullptr_t>(node.value)) {
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
    indent += indentStep;
}


void Writer::decIndent()
{
    indent -= indentStep;
}

