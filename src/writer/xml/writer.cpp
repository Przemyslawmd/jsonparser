
#include "writer/xml/writer.h"

#include <optional>
#include <variant>
#include <ranges>
#include <sstream>


using namespace xml;

std::string Writer::createXmlString(const std::vector<ElemWriter>& elems)
{
    std::ostringstream stream;
    for (const auto [idx, elem] : std::views::enumerate(elems))
    {
        switch(elem.type)
        {
            case ElemType::TAG_OPEN:
                incIndent();
                std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                stream << "<" << elem.name;
                if (!elem.attr.empty()) {
                    for (const auto& [key, val] : elem.attr) {
                        stream << " " << key << "=\"" << val << "\"";
                    }
                }
                stream << ">\n";
                break;;
            case ElemType::TAG_CLOSE:
                if (elems.at(idx - 1).type != ElemType::CONTENT) {
                    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                }
                stream << "</" << elem.name << ">";
                if (indent != 0) {
                    stream << "\n";
                }
                decIndent();
                break;
            case ElemType::CONTENT:
                deleteLastChars(stream, 1);
                stream << elem.name;
                break;
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

