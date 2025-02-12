
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


add_executable(
  jsonParserTest
  "testPreparser.cpp"
  "testParser.cpp"
  "testValidator.cpp"
  "testWriter.cpp"
  "testApiOtherActions.cpp"
  "testApiAddNode.cpp"
  "testApiChangeNode.cpp"
  "testApiRemoveNode.cpp"
  "testApiError.cpp"
  "../src/reader/Preparser.cpp"
  "../src/reader/Parser.cpp"
  "../src/reader/Validator.cpp"
  "../src/writer/Writer.cpp"
  "../src/log/Error.cpp"
  "../src/log/ErrorStorage.cpp"
  "../src/jsonApi.cpp"
  "../src/keyMapper.cpp"
  "../src/manager.cpp"
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

