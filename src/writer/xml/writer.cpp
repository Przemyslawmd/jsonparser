
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
        if (keyMapper.isAttrKey(idKey)) {
            deleteLastChars(stream, 2);
            stream << " " << keyMapper.getKeyStr(idKey).value() << "=" << "\"" << std::get<std::string>(val.value) << "\">" ;
            continue;
        }

        auto keyStr = keyMapper.getKeyStr(idKey);
        if (keyStr.value() == "__text") {
            parseData(val);
            enableIndent = false;
            decIndent();
            continue;
        }

        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
        stream << "<" << keyStr.value() << ">";
        parseData(val);
        stream << "</" << keyStr.value() << ">";
        if (indent != 0) {
            stream << "\n";
        }
    }
    if (enableIndent) {
        decIndent();
        std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
    }
    enableIndent = true;
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


void Writer::deleteLastChars(std::ostringstream& stream, uint noOfChars)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - noOfChars);
}


void Writer::incIndent()
{
    indent += indentStep;
}


void Writer::decIndent()
{
    indent -= indentStep;
}

