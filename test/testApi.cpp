
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


constexpr bool measurement = true;


class ApiTest : public testing::Test
{
protected:

    Utils utils;
    std::unique_ptr<JsonApi> prepareApi(const std::string& file);
};


std::unique_ptr<JsonApi> ApiTest::prepareApi(const std::string& file)
{
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, file);
    auto api = std::make_unique<JsonApi>();
    bool result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);
    return api;
}


TEST_F(ApiTest, AddIntValueToObjectDirectly)
{
    auto api = prepareApi("test_1.json");
    ObjectNode* root = api->getRoot();
    Node* node = &(root->at("person"));

    ObjectNode* nodePerson = std::get_if<ObjectNode>(&(node->value));
    nodePerson->insert(std::pair<std::string, int>("newValue", 23));

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddBoolValueToObjectDirectly)
{
    auto api = prepareApi("test_2.json");
    InnerNodePtr node = api->getNode({ "shipTo", "name" });
    ASSERT_FALSE(std::holds_alternative<nullptr_t>(node));

    (std::get<ObjectNode*>(node))->insert(std::pair<std::string, bool>("boolValue", true));

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_2_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddNestedObjectDirectly)
{
    auto api = prepareApi("test_2.json");
    InnerNodePtr node = api->getNode({ "shipTo", "name" });
    ASSERT_FALSE(std::holds_alternative<nullptr_t>(node));

    (std::get<ObjectNode*>(node))->insert(std::pair<std::string, bool>("boolValue", true));

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_2_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeValue)
{
    auto api = prepareApi("test_3.json");
    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeValueComplexJson)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_8_complex.json");

    bool result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);

    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{.value = "Maria"});
    ASSERT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_8_complex.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddSimpleNodeIntoObject)
{
    auto api = prepareApi("test_4.json");
    bool result = api->addNodeIntoObject({ "person2", "address" }, "post", Node{.value = "Cracow"});
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_4_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddSimpleNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->addNodeIntoArray({ "employees", size_t(0), "data", size_t(0) }, Node{.value = 4 });
    ASSERT_TRUE(result);

    result = api->insertNodeIntoArray({ "employees", size_t(1), "data", size_t(0) }, 1, Node{ .value = "c c"});
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddObjectNodeIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", std::vector<Node>{{ 0 }, { 100 }, { 200 }}));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddObjectNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("aa", "bb"));
    newObject.emplace(std::make_pair("cc", 12));

    bool result = api->insertNodeIntoArray({ "employees" , size_t(1), "data", size_t(0), size_t(0) }, 1, { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddArrayNodeIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_4.json");

    std::vector<Node> newArray{ { 232 }, { 234234 }, { 0 }, { 100 }};
    bool result = api->addNodeIntoObject({ "person2", "address"}, "dataArray", { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_4_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddArrayNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{{ 1 }, { 2 }, { 3 }};
    std::vector<Node> arr2{{ "aa" }, { "b" }};
    std::vector<Node> arr3{{ true }, { false }};
    std::vector<Node> newArray{{ arr1 }, { arr2 }, { arr3 }};
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveNodeFromObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_2.json");

    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);

    result = api->removeNodeFromObject({ "shipTo", "address" }, "cities");
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_2_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveNodeFromArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(1), "data" }, 1);
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeJSONForTheSameAPI)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{{ 1 }, { 2 }, { 3 }};
    std::vector<Node> arr2{{ "aa" }, { "b" }};
    std::vector<Node> arr3{{ true }, { false }};
    std::vector<Node> newArray{{ arr1 }, { arr2 }, { arr3 }};
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_3.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_8_complex.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);
    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{.value = "Maria"});
    ASSERT_TRUE(result);
    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    json = api->parseObjectToJsonString();
    jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_8_complex.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");
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

    json = api->parseObjectToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_4.json");
    ASSERT_EQ(json, jsonExpected);
}

