
#include <fstream>

#include <gtest/gtest.h>

#include "../src/jsonApi.h"
#include "config.h"
#include "utils.h"
#include <NodeValue.h>


constexpr bool measurement = true;
constexpr std::string_view logPrefix = "             ###### microseconds: ";


class ApiTest : public testing::Test
{
protected:

    std::unique_ptr<JsonApi> prepareApi(const std::string& file)
    {
        std::string jsonString = TestUtils::getJsonFromFile(TEST_DATA, file);
        auto api = std::make_unique<JsonApi>();
        bool result = api->parseJsonString(jsonString);
        EXPECT_TRUE(result);
        return api;
    }
};


/********************************************************************************/
/* ADD NODE INTO OBJECT *********************************************************/

TEST_F(ApiTest, AddSimpleNodeToObject)
{
    auto api = prepareApi("test_4.json");
    bool result = api->addNodeIntoObject({ "person2", "address" }, "post", Node{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_simple_node_to_object_4.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddObjectToObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("a", 123));
    newObject.emplace(std::make_pair("b", "AAA"));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddNestedObjectToObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", 12.45));

    std::map<std::string, Node> nestedObject;
    nestedObject.emplace(std::make_pair("qwe", "AA AA"));
    nestedObject.emplace(std::make_pair("asd", 345353));
    newObject.emplace(std::make_pair("internal", nestedObject));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_nested_object_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddNestedObjectWithArrayToObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_5.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("b", true));
    newObject.emplace(std::make_pair("a", std::vector<Node>{ { 0 }, { 100 }, { 200 } }));

    bool result = api->addNodeIntoObject({ "person" }, "newValues", { newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_nested_object_with_array_to_object_5.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, AddArrayToObject)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_4.json");

    std::vector<Node> newArray{ { 232 }, { 234234 }, { 0 }, { 100 } };
    bool result = api->addNodeIntoObject({ "person2", "address" }, "dataArrayNode", { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "add_array_to_object_4.json");
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

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addSimpleNodeToArray_7.json");
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

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addObjectToArray_7.json");
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

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "addArrayToArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* CHANGE NODE INTO OBJECT ******************************************************/

TEST_F(ApiTest, ChangeNodeInObjectIntoSimpleNode)
{
    auto api = prepareApi("test_3.json");
    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = "Spain" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoSimpleNode_3.json");
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

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeInObjectIntoArray)
{
    auto api = prepareApi("test_3.json");
    std::vector<Node> newArrayNode{ { true }, { "abv" }, { 0 }, { 1.01 }};

    bool result = api->changeNodeInObject({ "person" }, "country", Node{ .value = newArrayNode });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeNodeInObjectIntoArray_3.json");
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

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "changeComplexJson_8.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* CHANGE NODE IN ARAY **********************************************************/

TEST_F(ApiTest, ChangeNodeInArrayIntoSimpleNode)
{
    auto api = prepareApi("test_2.json");
    bool result = api->changeNodeInArray({ "shipTo", "cities" }, 2, Node{ .value = "Cracow" });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_simple_node_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeInArrayIntoObject)
{
    auto api = prepareApi("test_6.json");

    std::map<std::string, Node> newObject;
    newObject.emplace(std::make_pair("aa", "bb"));
    newObject.emplace(std::make_pair("xcd", true));
    newObject.emplace(std::make_pair("qwe", 33.45));
    newObject.emplace(std::make_pair("qaz", -12));

    bool result = api->changeNodeInArray({ "employees" }, size_t(0), Node{ .value = newObject });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_object_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ChangeNodeInArrayIntoArray)
{
    auto api = prepareApi("test_7.json");

    std::vector<Node> newArray{ { 1 }, { -100 }, { 43212231231 }};

    bool result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(0) }, size_t(0), Node{ .value = newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "change_node_in_array_into_array_7.json");
    ASSERT_EQ(json, jsonExpected);
}

/********************************************************************************/
/* REMOVE NODE FROM OBJECT ******************************************************/

TEST_F(ApiTest, RemoveSimpleNodeFromObject)
{
    auto api = prepareApi("test_2.json");

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "billTo" }, "name");
    ASSERT_TRUE(result);

    result = api->removeNodeFromObject({ "shipTo" }, "address" );
    const auto end = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeSimpleNodeFromObject_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveObjectFromObject)
{
    auto api = prepareApi("test_3.json");

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    const auto end = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {        
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeObjectFromObject_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveObjectAndAddForTheSameKey)
{
    auto api = prepareApi("test_3.json");

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->removeNodeFromObject({}, "person");
    ASSERT_TRUE(result);

    result = api->addNodeIntoObject({}, "person", Node{ .value = "newPerson" });
    const auto end = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {        
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeObjectAndAddForTheSameKey_3.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveArrayFromObject)
{
    auto api = prepareApi("test_7.json");

    const auto begin = std::chrono::high_resolution_clock::now();
    bool result = api->removeNodeFromObject({ "employees", size_t(1) }, "data");
    const auto end = std::chrono::high_resolution_clock::now();
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {        
        const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeArrayFromObject_7.json");
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

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeSimpleNodeFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveObjectFromArray)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_6.json");

    bool result = api->removeNodeFromArray({ "employees" }, size_t(0));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeObjectFromArray_6.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, RemoveArrayFromArray)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_7.json");

    bool result = api->removeNodeFromArray({ "employees", size_t(0), "data"}, size_t(1));
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "removeArrayFromArray_7.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, ClearApi)
{
    auto begin = std::chrono::high_resolution_clock::now();
    auto api = prepareApi("test_7.json");

    std::vector<Node> arr1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> arr2{ { "aa" }, { "b" } };
    std::vector<Node> arr3{ { true }, { false } };
    std::vector<Node> newArray{ { arr1 }, { arr2 }, { arr3 } };
    bool result = api->addNodeIntoArray({ "employees", size_t(1), "data" }, { newArray });
    ASSERT_TRUE(result);

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "clear_api_7.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    std::string jsonString = TestUtils::getJsonFromFile(TEST_DATA, "test_8_complex.json");
    result = api->parseJsonString(jsonString);
    EXPECT_TRUE(result);

    result = api->changeNodeInArray({ "employees", size_t(0), "data", size_t(1) }, 2, Node{ .value = 10 });
    ASSERT_TRUE(result);
    result = api->changeNodeInObject({ "employees", size_t(1), "employees", size_t(0) }, "name", Node{ .value = "Maria" });
    ASSERT_TRUE(result);
    result = api->changeNodeInArray({ "employees", size_t(1), "data", size_t(2), size_t(0), "numbers" }, 0, Node{ .value = 0.12 });
    ASSERT_TRUE(result);

    json = api->parseJsonObjectToString().value();
    jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "clear_api_8.json");
    ASSERT_EQ(json, jsonExpected);

    api->clear();
    jsonString = TestUtils::getJsonFromFile(TEST_DATA, "test_7.json");
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
    if (measurement) {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        std::cout << logPrefix << elapsed.count() << std::endl;
    }
    jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "clear_api_7_2.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObject_1)
{
    std::map<std::string, Node> internalObject;
    internalObject.insert(std::make_pair("age", 39));
    internalObject.insert(std::make_pair("country", "Poland"));
    internalObject.insert(std::make_pair("employed", true));
    internalObject.insert(std::make_pair("name", "John"));
    internalObject.insert(std::make_pair("restricted", false));
    internalObject.insert(std::make_pair("empty", nullptr));
    internalObject.insert(std::make_pair("newValue", 23.1));

    std::map<std::string, Node> root;
    root.insert(std::make_pair("person", internalObject));

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(Node{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "load_json_object_1.json");
    ASSERT_EQ(json, jsonExpected);
}


TEST_F(ApiTest, LoadJsonObject_2)
{
    std::vector<Node> array_1_1{ { 1 }, { 2 }, { 3 } };
    std::vector<Node> array_1_2{ { 4 }, { 5 }, { 6 } };
    std::vector<Node> array_1{ { array_1_1 }, { array_1_2 } };

    std::map<std::string, Node> object_1;
    object_1.emplace(std::make_pair("name", "Agata"));
    object_1.emplace(std::make_pair("data", array_1));

    std::vector<Node> array_2_1{ { "a" }, { "b" } };
    std::vector<Node> array_2_2{ { "c d e" } };
    std::vector<Node> array_2{ { array_2_1 }, { array_2_2 } };

    std::map<std::string, Node> object_2;
    object_2.emplace(std::make_pair("name", "Anna"));
    object_2.emplace(std::make_pair("data", array_2));

    std::vector<Node> mainArray;
    mainArray.emplace_back(object_1);
    mainArray.emplace_back(object_2);

    std::map<std::string, Node> root;
    root.emplace("employees", mainArray);

    auto api = std::make_unique<JsonApi>();
    api->loadJsonObject(Node{ .value = root });

    std::string json = api->parseJsonObjectToString().value();
    std::string jsonExpected = TestUtils::getJsonFromFile(TEST_DATA_API, "load_json_object_2.json");
    ASSERT_EQ(json, jsonExpected);
}

