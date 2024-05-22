
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


TEST_F(ApiTest, AddIntValueToObjectDirectly)
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


TEST_F(ApiTest, AddBoolValueToObjectDirectly_2)
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


TEST_F(ApiTest, AddNestedObjectDirectly)
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


TEST_F(ApiTest, ChangeNodeValueApi)
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


TEST_F(ApiTest, ChangeValueComplexJsonApi)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_8_complex.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", size_t(0), "data", size_t(1), size_t(2) }, Node{ .value = 10 });
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", size_t(1), "employees", size_t(0), "name"}, Node{ .value = "Maria" });
    ASSERT_TRUE(result);

    result = api.changeNodeValue({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers", size_t(0) }, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_8_complex.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddSimpleNodeIntoObjectApi)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_4.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    result = api.addNodeIntoObject({ "person2", "address" }, Node{ .value = "Cracow" }, "post" );
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddSimpleNodeIntoArrayApi)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    result = api.addNodeIntoArray({ "employees", size_t(0), "data", size_t(0) }, Node{.value = 4 });
    ASSERT_TRUE(result);

    result = api.addNodeIntoArray({ "employees", size_t(1), "data", size_t(0) }, Node{ .value = "c c"}, 1);
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddComplexNodeIntoObjectApi)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_5.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", std::vector<Node>{{ 0 }, { 100 }, { 200 }}));

    result = api.addNodeIntoObject({ "person" }, { newObject }, "newValues");
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_api_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddComplexNodeIntoArrayApi)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");

    JsonApi api;
    bool result = api.parseJsonString(jsonString);
    ASSERT_TRUE(result);

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("aa", "bb"));
    newObject.emplace(std::make_pair("cc", 12));

    result = api.addNodeIntoArray({ "employees" , size_t(1), "data", size_t(0), size_t(0) }, { newObject }, { 1 });
    ASSERT_TRUE(result);

    std::string json = api.parseObjectToJsonString(api.getRoot());
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA) + "api/", "test_api_7.json");
    ASSERT_EQ(json, jsonExpected);
}

