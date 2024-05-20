
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


class ApiTest : public testing::Test
{
protected:

    Utils utils;
};


TEST_F(ApiTest, InsertValue)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_1.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    ObjectNode* root = api.getRoot();
    Node* node = &(root->at("person"));

    ObjectNode* nodePerson = std::get_if<ObjectNode>(&(node->value));
    nodePerson->insert(std::pair<std::string, int>("newValue", 23));

    std::string json = api.parseObjectToJsonString(root);
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, InsertValue2)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_2.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    InnerNodePtr node = api.getNode({ "shipTo", "name" });
    ASSERT_FALSE(std::holds_alternative<nullptr_t>(node));

    (std::get<ObjectNode*>(node))->insert(std::pair<std::string, bool>("boolValue", true));

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeValue)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_3.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "person", "country" }, Node{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeValueComplex)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_8_complex.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", (size_t) 0, "data", (size_t) 1, (size_t) 2 }, Node{ .value = 10 });
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", (size_t) 1, "employees", (size_t) 0, "name"}, Node{ .value = "Maria" });
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", (size_t) 1, "data", (size_t) 2, (size_t) 0, "numbers", (size_t) 0 }, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_8_complex.json");
    ASSERT_EQ(json, jsonExpected);
}

