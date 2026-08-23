
#include "writer/xml/writer.h"

#include <optional>
#include <variant>
#include <ranges>
#include <sstream>

#include "definesXML.h"


using namespace xml;

std::string Writer::createXmlString(std::unique_ptr<std::vector<ElemWriter>> elems)
{
    std::ostringstream stream;

    const auto& attrsDec = keyMapper.getAttrsDec();
    if (!attrsDec.empty()) {
        stream << "<?xml ";
        if (attrsDec.contains(VER)) {
            stream << VER << "=" << "\"" << attrsDec.at(VER) << "\" ";
        }
        if (attrsDec.contains(ENC)) {
            stream << ENC << "=" << "\"" << attrsDec.at(ENC) << "\" ";
        }
        if (attrsDec.contains(STA)) {
            stream << STA << "=" << "\"" << attrsDec.at(STA) << "\" ";
        }
        deleteLastChars(stream, 1);
        stream << "?>\n";
    }

    unsigned int numOfElems = elems->size();
    for (const auto [idx, elem] : std::views::enumerate(*elems))
    {
        switch(elem.type)
        {
            case ElemType::TAG_OPEN:
                incIndent();
                std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
                stream << "<" << elem.name.value();
                if (!elem.attr.empty()) {
                    for (const auto& [key, val] : elem.attr) {
                        stream << " " << key << "=\"" << val << "\"";
                    }
                }
                stream << ">\n";
                break;;
            case ElemType::TAG_CLOSE:
                if (elems->at(idx - 1).type != ElemType::CONTENT) {
                    std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
                }
                stream << "</" << elem.name.value() << ">";
                if (idx != (numOfElems - 1)) {
                    stream << "\n";
                }
                decIndent();
                break;
            case ElemType::TAG_ARRAY_BEGIN:
                incIndent();
                std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
                stream << "<" << elem.name.value() <<">\n";
                break;
            case ElemType::TAG_ARRAY_OPEN:
                std::fill_n(std::ostream_iterator<char>(stream), indentation, ' ');
                stream << "<" << elem.name.value() << ">\n";
                break;
            case ElemType::TAG_ARRAY_CLOSE:
                stream << "</" << elem.name.value() << ">\n";
                break;
            case ElemType::TAG_ARRAY_END:
                stream << "</" << elem.name.value() << ">\n";
                decIndent();
                break;
            case ElemType::CONTENT:
                deleteLastChars(stream, 1);
                std::visit([&stream](const auto& val) { stream << val; }, elem.value);
                break;
        }
    }
    return stream.str();
}


void Writer::setIndent(int step)
{
    this->indentationStep = step;
}

/*******************************************************************/
/* PRIVATE *********************************************************/

void Writer::deleteLastChars(std::ostringstream& stream, unsigned int noOfChars)
{
    size_t pos = stream.tellp();
    stream.seekp(pos - noOfChars);
}


void Writer::incIndent()
{
    indentation += indentationStep;
}


void Writer::decIndent()
{
    indentation -= indentationStep;
}

