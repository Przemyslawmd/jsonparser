
#include "Preparser.h"


Preparser::Preparser()
{
    tokens = std::make_unique<std::vector<Token>>();
} 


std::unique_ptr<std::vector<Token>> Preparser::parseJSON(const std::string& json)
{
    return nullptr;
}


void Preparser::parseNumber()
{
}


void Preparser::parseString()
{
}

