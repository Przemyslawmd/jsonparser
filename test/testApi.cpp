
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

/********************************************************************************/
/* ADD NODE INTO OBJECT *********************************************************/

TEST_F(ApiTest, AddSimpleNodeIntoObject)
{
    auto api = prepareApi("test_4.json");
    bool result = api->addNodeIntoObject({ "person2", "address" }, "post", Node{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addSimpleNodeToObject_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddObjectIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("a", 123));
    newObjectNode.emplace(std::make_pair("b", "AAA"));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addObjectToObject_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddNestedObjectIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("b", true));
    newObjectNode.emplace(std::make_pair("a", 12.45));

    std::map<std::string, Node> nestedObjectNode;
    nestedObjectNode.emplace(std::make_pair("qwe", "AA AA"));
    nestedObjectNode.emplace(std::make_pair("asd", 345353));
    newObjectNode.emplace(std::make_pair("internal", nestedObjectNode));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addNestedObjectToObject_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddNestedObjectWithArrayIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("b", true));
    newObjectNode.emplace(std::make_pair("a", std::vector<Node>{{ 0 }, { 100 }, { 200 }}));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addNestedObjectWithArrayToObject_5.json");
    std::cout << json << std::endl;
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddArrayIntoObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_4.json");

    std::vector<Node> newArrayNode{ { 232 }, { 234234 }, { 0 }, { 100 } };
    bool result = api->addNodeIntoObject({ "person2", "address" }, "dataArrayNode", { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addArrayToObject_4.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* ADD NODE INTO ARRAY **********************************************************/

TEST_F(ApiTest, AddSimpleNodeIntoArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->addNodeIntoArray({ "employees", size_t(0), "data", size_t(0) }, Node{ .value = 4 });
    ASSERT_TRUE(result);

    result = api->insertNodeIntoArray({ "employees", size_t(1), "data", size_t(0) }, 1, Node{ .value = "c c" });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addSimpleNodeToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddObjectIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("aa", "bb"));
    newObjectNode.emplace(std::make_pair("cc", 12));

    bool result = api->insertNodeIntoArray({ "employees" , size_t(1), "data", size_t(0) }, 1, { newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addObjectToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddArrayIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> arr2{ { "aa" }, { "b" } };
    std::vector<Node> arr3{ { true }, { false } };
    std::vector<Node> newArrayNode{ { arr1 }, { arr2 }, { arr3 } };
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "addArrayToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* CHANGE NODE INTO OBJECT ******************************************************/

TEST_F(ApiTest, ChangeNodeInObjectIntoSimpleNode)
{
    auto api = prepareApi("test_3.json");
    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "changeNodeInObjectIntoSimpleNode_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeInObjectIntoObject)
{
    auto api = prepareApi("test_3.json");

    std::map<std::string, Node> newObjectNode;
    newObjectNode.emplace(std::make_pair("b", true));
    newObjectNode.emplace(std::make_pair("a", 12.45));
    newObjectNode.emplace(std::make_pair("--", "........."));

    std::map<std::string, Node> nestedObjectNode;
    nestedObjectNode.emplace(std::make_pair("**", "ccccccccc"));
    nestedObjectNode.emplace(std::make_pair("^^", -12));
    newObjectNode.emplace(std::make_pair("internal", nestedObjectNode));

    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = newObjectNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "changeNodeInObjectIntoObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeInObjectIntoArray)
{
    auto api = prepareApi("test_3.json");
    std::vector<Node> newArrayNode{ { true }, { "abv" }, { 0 }, { 1.01 }};

    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "changeNodeInObjectIntoArray_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeComplexJson)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_8_complex.json");

    bool result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);

    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{.value = "Maria"});
    ASSERT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "changeComplexJson_8.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* REMOVE NODE FROM OBJECT ******************************************************/

TEST_F(ApiTest, RemoveSimpleNodeFromObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_2.json");

    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);

    result = api->removeNodeFromObject({ "shipTo" }, "address" );
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* REMOVE NODE FROM ARRAY *******************************************************/

TEST_F(ApiTest, RemoveSimpleNodeFromArray)
{
    auto api = prepareApi("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);

    result = api->removeNodeFromArray({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "removeSimpleNodeFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

/*
TEST_F(ApiTest, ChangeJSONForTheSameAPI)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{{ 1 }, { 2 }, { 3 }};
    std::vector<Node> arr2{{ "aa" }, { "b" }};
    std::vector<Node> arr3{{ true }, { false }};
    std::vector<Node> newArrayNode{{ arr1 }, { arr2 }, { arr3 }};
    bool result = api->addNodeIntoArrayNode({ "employees", size_t(1), "data" }, { newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_3.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    std::string jsonString = utils.getJsonFromFile(TEST_DATA, "test_8_complex.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->changeNodeInArrayNode({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);
    result = api->changeNodeInObjectNode({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{.value = "Maria"});
    ASSERT_TRUE(result);
    result = api->changeNodeInArrayNode({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    json = api->parseObjectNodeToJsonString();
    jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_8_complex.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    jsonString = utils.getJsonFromFile(TEST_DATA, "test_7.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->removeNodeFromArrayNode({ "employees", size_t(0), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArrayNode({ "employees", size_t(0), "data", size_t(1) }, 0);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArrayNode({ "employees", size_t(1), "data", size_t(0) }, 1);
    ASSERT_TRUE(result);
    result = api->removeNodeFromArrayNode({ "employees", size_t(1), "data" }, 1);
    ASSERT_TRUE(result);

    json = api->parseObjectNodeToJsonString();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << "             ###### microseconds: " << elapsed.count() << std::endl;
    }
    jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_API), "test_api_7_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObjectNode_1)
{
    std::unique_ptr<ObjectNode> obj = std::make_unique<ObjectNode>();
    obj->insert(std::make_pair<std::string, ObjectNode>("person", { ObjectNode() }));

    ObjectNode* personObj = std::get_if<ObjectNode>(&obj->at("person").value);
    personObj->insert(std::make_pair("age", 39));
    personObj->insert(std::make_pair("country", "Poland"));
    personObj->insert(std::make_pair("employed", true));
    personObj->insert(std::make_pair("name", "John"));
    personObj->insert(std::make_pair("restricted", false));
    personObj->insert(std::make_pair("empty", nullptr));

    auto api = std::make_unique<JsonApi>();
    api->loadObjectNode(std::move(obj));

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_WRITER), "test_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObjectNode_2)
{
    std::unique_ptr<ObjectNode> obj = std::make_unique<ObjectNode>();
    obj->insert(std::make_pair<std::string, ObjectNode>("person", { ObjectNode() }));
    obj->insert(std::make_pair<std::string, std::string>("company", "abc"));
    obj->insert(std::make_pair<std::string, ArrayNode>("cities", { ArrayNode() }));

    ObjectNode* personObj = std::get_if<ObjectNode>(&obj->at("person").value);
    personObj->insert(std::make_pair("age", 39));
    personObj->insert(std::make_pair("country", "Poland"));
    personObj->insert(std::make_pair("name", "John"));

    ArrayNode* citiesArr = std::get_if<ArrayNode>(&obj->at("cities").value);
    citiesArr->emplace_back("Krakow");
    citiesArr->emplace_back("Warszawa");
    citiesArr->emplace_back("Wroclaw");
    citiesArr->emplace_back("Poznan");

    auto api = std::make_unique<JsonApi>();
    api->loadObjectNode(std::move(obj));

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_WRITER), "test_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObjectNode_3)
{
    std::unique_ptr<ObjectNode> obj = std::make_unique<ObjectNode>();
    obj->insert(std::make_pair<std::string, ArrayNode>("employees", { ArrayNode() }));

    ArrayNode* employeesArr = std::get_if<ArrayNode>(&obj->at("employees").value);

    ObjectNode objAgata;
    objAgata.insert(std::make_pair<std::string, int>("age", 33));
    objAgata.insert(std::make_pair<std::string, std::string>("email", "agata@gmail.com"));
    objAgata.insert(std::make_pair<std::string, std::string>("name", "Agata"));
    employeesArr->emplace_back(objAgata);

    ObjectNode objAnna;
    objAnna.insert(std::make_pair<std::string, int>("age", 31));
    objAnna.insert(std::make_pair<std::string, std::string>("email", "anna@gmail.com"));
    objAnna.insert(std::make_pair<std::string, std::string>("name", "Anna"));
    employeesArr->emplace_back(objAnna);

    auto api = std::make_unique<JsonApi>();
    api->loadObjectNode(std::move(obj));

    std::string json = api->parseObjectNodeToJsonString();
    std::string jsonExpected = utils.getJsonFromFile(std::string(TEST_DATA_WRITER), "test_6.json");
    ASSERT_EQ(json, jsonExpected);
}
*/

