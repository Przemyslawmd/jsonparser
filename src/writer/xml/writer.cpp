
#include "writer/xml/writer.h"

#include <optional>
#include <variant>
#include <ranges>
#include <sstream>

#include "definesXML.h"

using namespace xml;
using enum ElemType;


std::string Writer::createXmlString(std::unique_ptr<std::vector<Elem>> elems)
{
    std::ostringstream stream;
    unsigned int indent = 0;

    if (const auto& attrsDesc = keyMapper.getAttrsDec(); !attrsDesc.empty()) {
        stream << "<?xml ";
        if (attrsDesc.contains(VER)) {
            stream << VER << "=" << "\"" << attrsDesc.at(VER) << "\" ";
        }
        if (attrsDesc.contains(ENC)) {
            stream << ENC << "=" << "\"" << attrsDesc.at(ENC) << "\" ";
        }
        if (attrsDesc.contains(STA)) {
            stream << STA << "=" << "\"" << attrsDesc.at(STA) << "\" ";
        }
        deleteLastChars(stream, 1);
        stream << "?>\n";
    }

    const unsigned int numOfElems = elems->size();
    for (const auto [idx, elem] : std::views::enumerate(*elems))
    {
        switch(elem.type)
        {
            case TAG_OPEN:
                if (idx != 0) {
                    indent += indentStep;
                }
                std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                stream << "<" << elem.name.value();
                if (!elem.attrs.empty()) {
                    for (const auto& attr : elem.attrs) {
                        stream << " " << std::get<0>(attr) << "=\"" << std::get<1>(attr) << "\"";
                    }
                }
                stream << ">\n";
                break;
            case TAG_CLOSE:
                if (elems->at(idx - 1).type != CONTENT) {
                    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                }
                stream << "</" << elem.name.value() << ">";
                if (idx != (numOfElems - 1)) {
                    stream << "\n";
                }
                indent -= indentStep;
                break;
            case TAG_ARRAY_BEGIN:
                indent += indentStep;
                std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                stream << "<" << elem.name.value() <<">\n";
                break;
            case TAG_ARRAY_OPEN:
                std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                stream << "<" << elem.name.value() << ">\n";
                break;
            case TAG_ARRAY_CLOSE:
                if (elems->at(idx - 1).type != CONTENT) {
                    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                }
                stream << "</" << elem.name.value() << ">\n";
                break;
            case TAG_ARRAY_END:
                if (elems->at(idx - 1).type != CONTENT) {
                    std::fill_n(std::ostream_iterator<char>(stream), indent, ' ');
                }
                stream << "</" << elem.name.value() << ">\n";
                indent -= indentStep;
                break;
            case CONTENT:
                deleteLastChars(stream, 1);
                std::visit([&stream](const auto& val) { stream << val; }, elem.value);
                break;
        }
    }
    return std::move(stream).str();
}


void Writer::setIndent(unsigned int step)
{
    this->indentStep = step;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::deleteLastChars(std::ostringstream& stream, unsigned int noOfChars)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - noOfChars);
}

