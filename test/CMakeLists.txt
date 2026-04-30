
project(jsonParserTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)


if(UNIX)
    set(TIME_MEASUREMENT_TYPE std::chrono::system_clock::time_point)
elseif(WIN32)
    set(TIME_MEASUREMENT_TYPE std::chrono::steady_clock::time_point)
endif()


configure_file (timeType.h.in ${CMAKE_CURRENT_SOURCE_DIR}/timeType.h @ONLY)
configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)


set(TEST_FILES "")

if (JSON_BUILD)
    list(APPEND TEST_FILES "testPreparser.cpp")
    list(APPEND TEST_FILES "testParser.cpp")
    list(APPEND TEST_FILES "testValidator.cpp")
    list(APPEND TEST_FILES "testWriter.cpp")
    list(APPEND TEST_FILES "testApiOtherActions.cpp")
    list(APPEND TEST_FILES "testApiAddNode.cpp")
    list(APPEND TEST_FILES "testApiChangeNode.cpp")
    list(APPEND TEST_FILES "testApiRemoveNode.cpp")
    list(APPEND TEST_FILES "testApiError.cpp")
endif()

if (XML_BUILD)
    list(APPEND TEST_FILES "xml/testPreparserXML.cpp")
endif()


add_executable(
  jsonParserTest
  "../src/manager.cpp"
  "../src/reader/Preparser.cpp"
  "../src/reader/Validator.cpp"
  "../src/reader/Parser.cpp"
  "../src/writer/Writer.cpp"
  "../src/log/Error.cpp"
  "../src/log/ErrorStorage.cpp"
  "../src/jsonApi.cpp"
  "../src/keyMapper.cpp"
  ${TEST_FILES}
)


target_compile_features(jsonParserTest PRIVATE cxx_std_20)

target_include_directories(
  jsonParserTest 
  PRIVATE 
  googletest/include 
  "../headers/" 
  "../src/headers/" 
  "../src/"
)

target_link_libraries(
  jsonParserTest
  GTest::gtest_main  
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(jsonParserTest)

