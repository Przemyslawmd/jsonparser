
#include "writer.h"

#include <iterator>
#include <optional>
#include <variant>


constexpr std::string_view DATA_END = ",\n";

static void deleteLastChars(std::ostringstream& stream)
{
    const size_t pos = stream.tellp();
    stream.seekp(pos - 2);
    stream << "\n";
}


using namespace json;

std::string Writer::createJsonString(const ObjectNode& object)
{
    processObjectNode(object);
    return stream.str();
}


void Writer::setIndent(unsigned int step)
{
    indentStep = step;
}


/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::processObjectNode(const ObjectNode& obj)
{
    stream << "{\n";
    incIndent();

    for (auto const& [keyID, node] : obj) {
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        auto key = keyMapper.getKeyStr(keyID);
        stream << "\"" << key.value() << "\": ";
        parseData(node.value);
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

    for (const auto& node : arr) {
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        parseData(node.value);
    }
    deleteLastChars(stream);

    decIndent();
    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
    stream << "],\n";
}


void Writer::parseData(const Node::Value& val)
{
    if (std::holds_alternative<std::string>(val)) {
        stream << "\"" << std::get<std::string>(val) << "\"" << DATA_END;
    }
    else if (std::holds_alternative<int64_t>(val)) {
        stream << std::get<int64_t>(val) << DATA_END;
    }
    else if (std::holds_alternative<double>(val)) {
        stream << std::get<double>(val) << DATA_END;
    }
    else if (std::holds_alternative<bool>(val)) {
        stream << (std::get<bool>(val) == true ? "true" : "false") << DATA_END;
    }
    else if (std::holds_alternative<std::nullptr_t>(val)) {
        stream << "null" << DATA_END;
    }
    else if (std::holds_alternative<ObjectNode>(val)) {
        processObjectNode(std::get<ObjectNode>(val));
    }
    else if (std::holds_alternative<ArrayNode>(val)) {
        processArrayNode(std::get<ArrayNode>(val));
    }
}


void Writer::incIndent()
{
    indent += indentStep;
}


void Writer::decIndent()
{
    indent -= indentStep;
}

