
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
                stream << "<" << elem.name.value();
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
                stream << "</" << elem.name.value() << ">";
                if (indent != 0) {
                    stream << "\n";
                }
                decIndent();
                break;
            case ElemType::CONTENT:
                deleteLastChars(stream, 1);
                if (std::holds_alternative<std::string>(elem.value)) {
                    stream << std::get<std::string>(elem.value);
                }
                else if (std::holds_alternative<int64_t>(elem.value))
                {
                    stream << std::get<int64_t>(elem.value);
                }
                else if (std::holds_alternative<double>(elem.value))
                {
                    stream << std::get<double>(elem.value);
                }
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

