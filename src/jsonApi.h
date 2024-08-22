
#ifndef JSONPARSER_API_H
#define JSONPARSER_API_H

#include <memory>
#include <string>
#include <vector>

#include <Error.h>
#include "manager.h"


/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


class JsonApi
{
public:
    JsonApi();

    bool parseJsonString(const std::string& file);

    std::string parseJsonObjectToString();

    bool loadJsonObject(const Node&);

    void clear();

    bool addNodeIntoObject(const std::vector<Path>&, const std::string& keyStr, const Node&);
    bool addNodeIntoArray(const std::vector<Path>&, const Node&);
    bool insertNodeIntoArray(const std::vector<Path>&, size_t index, const Node&);

    bool changeNodeInObject(const std::vector<Path>&, const std::string& keyStr, const Node&);
    bool changeNodeInArray(const std::vector<Path>&, size_t index, const Node&);

    bool removeNodeFromObject(const std::vector<Path>&, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>&, size_t index);

    ErrorCode getErrorCode();

private:
    std::unique_ptr<Manager> manager;
};

#endif

