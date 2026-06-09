
#include "writer/xml/writer.h"

#include <iterator>
#include <optional>
#include <variant>


constexpr std::string_view DATA_END = ">\n";

using namespace xml;

std::string Writer::createXmlString(const ObjectNode& object)
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
    incIndent();
    for (auto const& [idKey, val] : obj) {
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        auto keyStr = keyMapper.getKeyStr(idKey);
        stream << "<" << keyStr.value() << ">";
        parseData(val);
        stream << "</" << keyStr.value() << ">";
        if (indent != 0) {
            stream << "\n";
        }
    }
    decIndent();
    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
}


void Writer::parseData(const Node& node)
{
    if (std::holds_alternative<std::string>(node.value)) {
        stream << std::get<std::string>(node.value);
    }
    else if (std::holds_alternative<bool>(node.value)) {
        stream << (std::get<bool>(node.value) == true ? "true" : "false") << DATA_END;
    }
    else if (std::holds_alternative<nullptr_t>(node.value)) {
        stream << "null" << DATA_END;
    }
    else if (std::holds_alternative<ObjectNode>(node.value)) {
        stream << '\n';
        processObjectNode(std::get<ObjectNode>(node.value));
    }
}


void Writer::deleteLastChars(std::ostringstream& stream)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - 2);
}


void Writer::incIndent()
{
    indent += indentStep;
}


void Writer::decIndent()
{
    indent -= indentStep;
}

