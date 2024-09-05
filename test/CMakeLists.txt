
project(jsonParserTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(HEADERS "../headers")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

configure_file(${PROJECT_SOURCE_DIR}/config.h.in ${PROJECT_SOURCE_DIR}/config.h)

if(UNIX)
    set(TIME_MEASUREMENT_TYPE std::chrono::system_clock::time_point)
elseif(WIN32)
    set(TIME_MEASUREMENT_TYPE std::chrono::steady_clock::time_point)
endif()

add_executable(
  jsonParserTest
  testPreparser.cpp
  testParser.cpp
  testValidator.cpp
  testWriter.cpp
  testApi.cpp
  testApiAddingNode.cpp
  testApiError.cpp
  ../src/jsonApi.cpp
  ../src/reader/Preparser.cpp
  ../src/reader/Parser.cpp
  ../src/reader/ParserKey.cpp
  ../src/reader/Validator.cpp
  ../src/writer/Writer.cpp
  ../src/keyMapper.cpp
  ../src/utils.cpp
  ../src/manager.cpp
  ../src/log/Error.cpp
)

target_compile_features(jsonParserTest PRIVATE cxx_std_20)

target_include_directories(
  jsonParserTest 
  PRIVATE 
  googletest/include 
  ${HEADERS}
)

target_link_libraries(
  jsonParserTest
  GTest::gtest_main  
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(jsonParserTest)

