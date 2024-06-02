
#include <map>
#include <string>

#include "../headers/defines.h"


class Message
{
public:
    const std::string& getMessage(Result result);

private:
    const std::map<Result, std::string> messages = 
    {
        { Result::API_EMPTY,
            "There is no JSON object in JsonApi, parse JSON string or load JSON object"},
        { Result::API_NOT_EMPTY,
            "Action requires JsonApi instance without JSON object, make clear" },
        { Result::API_NOT_KEY_IN_MAP,
            "There is no provided key in object" },
        { Result::API_INCONSISTENT_DATA,
            "Using key for array or index for object" },
        { Result::API_INDEX_OUT_OF_ARRAY,
            "There is no provided index in array" },
        { Result::API_NODE_NOT_ARRAY,
            "Expected array, but Node is not array" },
        { Result::API_NODE_NOT_OBJECT, 
            "Expected object, but Node is not object" }
    };
};

