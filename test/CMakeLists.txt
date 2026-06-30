
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


if (JSON_BUILD)
    list(APPEND TEST_FILES "json/testPreparserJSON.cpp")
    list(APPEND TEST_FILES "json/testParserJSON.cpp")
    list(APPEND TEST_FILES "json/testValidatorJSON.cpp")
    list(APPEND TEST_FILES "json/testWriterJSON.cpp")
    list(APPEND TEST_FILES "testApiOtherActions.cpp")
    list(APPEND TEST_FILES "json/testApiAddNodeJSON.cpp")
    list(APPEND TEST_FILES "json/testApiChangeNodeJSON.cpp")
    list(APPEND TEST_FILES "testApiRemoveNode.cpp")
    list(APPEND TEST_FILES "testApiError.cpp")

    list(APPEND SRC_FILES "../src/manager.cpp")
    list(APPEND SRC_FILES "../src/reader/json/preparser.cpp")
    list(APPEND SRC_FILES "../src/reader/json/validator.cpp")
    list(APPEND SRC_FILES "../src/reader/json/parser.cpp")
    list(APPEND SRC_FILES "../src/writer/json/writer.cpp")
    list(APPEND SRC_FILES "../src/log/Error.cpp")
    list(APPEND SRC_FILES "../src/log/ErrorStorage.cpp")
    list(APPEND SRC_FILES "../src/jsonApi.cpp")
    list(APPEND SRC_FILES "../src/keyMapper.cpp")
endif()

if (XML_BUILD)
    list(APPEND TEST_FILES "xml/testPreparser.cpp")
    list(APPEND TEST_FILES "xml/testParserTokens.cpp")
    list(APPEND TEST_FILES "xml/testValidator.cpp")
    list(APPEND TEST_FILES "xml/testObjectCreator.cpp")
    list(APPEND TEST_FILES "xml/testWriterXML.cpp")
    list(APPEND TEST_FILES "xml/testWriterElemCreator.cpp")

    list(APPEND SRC_FILES "../src/reader/xml/preparser.cpp")
    list(APPEND SRC_FILES "../src/reader/xml/parserTokensXML.cpp")
    list(APPEND SRC_FILES "../src/reader/xml/objectCreator.cpp")
    list(APPEND SRC_FILES "../src/writer/xml/writer.cpp")
    list(APPEND SRC_FILES "../src/writer/xml/elemWriterCreator.cpp")
endif()

if (JSON_BUILD AND XML_BUILD)
    list(APPEND TEST_FILES "testApiXmlToJson.cpp")
endif()

add_executable(
  jsonParserTest
  ${SRC_FILES}
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
  "./"
)

target_link_libraries(
  jsonParserTest
  GTest::gtest_main
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(jsonParserTest)

