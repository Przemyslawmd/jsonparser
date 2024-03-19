
#ifndef JSONCPP_TOKENVALUE_H
#define JSONCPP_TOKENVALUE_H

struct TokenValue
{
    using Value = std::variant<std::string, int, std::map<std::string, TokenValue>>;
    Value value;
};

#endif

