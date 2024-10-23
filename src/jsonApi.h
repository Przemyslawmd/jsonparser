
#ifndef JSONPARSER_API_H
#define JSONPARSER_API_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "error.h"
#include "manager.h"


/* Path contains a key for an object (map) or index for an array */
using Path = std::variant<std::string, size_t>;


class JsonApi
{
public:
    JsonApi();

    bool parseJsonString(const std::string& file);

    std::optional<std::string> parseJsonObjectToString();

    bool loadJsonObject(const NodeApi&);
    bool isJsonObject();

    void clear();

    bool addNodeIntoObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool addNodeIntoArray(const std::vector<Path>&, const NodeApi&);
    bool insertNodeIntoArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool changeNodeInObject(const std::vector<Path>&, const std::string& keyStr, const NodeApi&);
    bool changeNodeInArray(const std::vector<Path>&, size_t index, const NodeApi&);

    bool removeNodeFromObject(const std::vector<Path>&, const std::string& keyStr);
    bool removeNodeFromArray(const std::vector<Path>&, size_t index);

    const std::vector<Error>& getErrors();

private:
    std::unique_ptr<Manager> manager;
};

#endif

