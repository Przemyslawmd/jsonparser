
#include <chrono>

#include <gtest/gtest.h>

#include "log/ErrorStorage.h"
#include "jsonApi.h"

#include "baseTest.h"
#include "config.h"
#include "utils.h"
#include "node.h"


using std::chrono::high_resolution_clock;


class ApiOtherActions : public BaseTest {};


TEST_F(ApiOtherActions, ClearApi)
{
    auto api = prepareApi("test_7.json");

    std::vector<NodeApi> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<NodeApi> arr2{ { "aa" }, { "b" } };
    std::vector<NodeApi> arr3{ { true }, { false } };
    std::vector<NodeApi> newArray{ { arr1 }, { arr2 }, { arr3 } };

    const auto begin = high_resolution_clock::now();
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_7.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    std::string jsonString = getJsonFromFile(TEST_DATA, "test_8_complex.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, NodeApi{ .value = 10 });
    ASSERT_TRUE(result);
    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", NodeApi{ .value = "Maria" });
    ASSERT_TRUE(result);
    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, NodeApi{ .value = 0.12 });
    ASSERT_TRUE(result);

    json = api->parseJsonObjectToString().value();
    jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_8.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    jsonString = getJsonFromFile(TEST_DATA, "test_7.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArray({ "employees", size_t(1), "data" }, 1);
    ASSERT_TRUE(result);

    json = api->parseJsonObjectToString().value();
    const auto end = high_resolution_clock::now();
    showDuration(begin, end);
    jsonExpected = getJsonFromFile(TEST_DATA_API, "clear_api_7_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiOtherActions, LoadJsonObject_1)
{
    std::map<std::string, NodeApi> internalObject;
    internalObject.emplace("age", 39);
    internalObject.emplace("country", "Poland");
    internalObject.emplace("employed", true);
    internalObject.emplace("name", "John");
    internalObject.emplace("restricted", false);
    internalObject.emplace("empty", nullptr);
    internalObject.emplace("newValue", 23.1);

    std::map<std::string, NodeApi> root;
    root.emplace("person", internalObject);

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(NodeApi{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "load_json_object_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiOtherActions, LoadJsonObject_2)
{
    std::vector<NodeApi> array_1_1{ { 1 }, { 2 }, { 3 } };
    std::vector<NodeApi> array_1_2{ { 4 }, { 5 }, { 6 } };
    std::vector<NodeApi> array_1{ { array_1_1 }, { array_1_2 } };

    std::map<std::string, NodeApi> object_1;
    object_1.emplace("name", "Agata");
    object_1.emplace("data", array_1);

    std::vector<NodeApi> array_2_1{ { "a" }, { "b" } };
    std::vector<NodeApi> array_2_2{ { "c d e" } };
    std::vector<NodeApi> array_2{ { array_2_1 }, { array_2_2 } };

    std::map<std::string, NodeApi> object_2;
    object_2.emplace("name", "Anna");
    object_2.emplace("data", array_2);

    std::vector<NodeApi> mainArray;
    mainArray.emplace_back(object_1);
    mainArray.emplace_back(object_2);

    std::map<std::string, NodeApi> root;
    root.emplace("employees", mainArray);

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(NodeApi{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = getJsonFromFile(TEST_DATA_API, "load_json_object_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiOtherActions, LoadJsonObject_Error)
{
    std::vector<NodeApi> arrayNode{ { 1 }, { 2 }, { 3 } };

    auto api = std::make_unique<JsonApi>();
    bool result = api->loadJsonObject(NodeApi{ .value = arrayNode });
    ASSERT_FALSE(result);
    ASSERT_FALSE(api->isJsonObject());

    const auto& errors = ErrorStorage::getErrors();
    ASSERT_EQ(errors.at(0).getCode(), ErrorCode::MANAGER_ROOT_NOT_OBJECT);
}

