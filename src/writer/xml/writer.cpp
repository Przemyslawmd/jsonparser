
#include "writer/xml/writer.h"

#include <iterator>
#include <optional>
#include <variant>


constexpr std::string_view DATA_END = ">\n";

using namespace xml;

#include <iostream>

std::string Writer::createXmlString(const std::vector<ElemWriter>& elems)
{
    for (uint i = 0; i < elems.size(); i++)
    {
        if (elems.at(i).type == ElemType::TAG_OPEN) {
            incIndent();
            std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
            stream << "<" << elems.at(i).name;
            if (!elems.at(i).attr.empty()) {
                for (const auto& [key, val] : elems.at(i).attr) {
                    stream << " " << key << "=\"" << val << "\"";
                }
            }
            stream << ">\n";
        }
        else if (elems.at(i).type == ElemType::TAG_CLOSE) {
            if (elems.at(i - 1).type != ElemType::CONTENT) {
                std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
            }
            stream << "</" << elems.at(i).name << ">";
            if (indent != 0) {
                stream << "\n";
            }
            decIndent();
        }
        else if (elems.at(i).type == ElemType::CONTENT) {
            deleteLastChars(stream, 1);
            stream << elems.at(i).name;
        }
    }
    return stream.str();
}


void Writer::setIndent(size_t indentStep)
{
    this->indentStep = indentStep;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

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

