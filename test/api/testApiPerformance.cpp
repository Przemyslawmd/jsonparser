
#include <chrono>

#include <gtest/gtest.h>

#include "jsonApi.h"

#include "baseTest.h"
#include "utilsTest.h"
#include "node.h"


using uint = unsigned int;

namespace
{
    class  TestApiPerformance : public BaseTest {};
}

TEST_F(TestApiPerformance, RemoveAndAddNode)
{
    const auto api = prepareApiWithJson("test_8_complex.json");
    const std::vector<Path> path = { "employees",
                                      static_cast<uint>(1),
                                      "data",
                                      static_cast<uint>(2),
                                      static_cast<uint>(0),
                                      "numbers" };

    const auto begin = std::chrono::high_resolution_clock::now();
    for (unsigned int i = 0; i < NUM_OF_TESTS; i++) {
        bool result = api->removeNodeFromArray(path, 1);
        ASSERT_TRUE(result);
        result = api->insertNodeIntoArray(path, 1, { 4 });
        ASSERT_TRUE(result);
    }
    const auto end = std::chrono::high_resolution_clock::now();
    showDuration(begin, end);
}

