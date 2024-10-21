
project(jsonParserTest)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(HEADERS_API "../headers/")
set(HEADERS "../src/headers/")
set(SRC "../src")
set(LOG "../src/log")
set(READER "../src/reader")
set(WRITER "../src/writer")

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
  testPreparser.cpp 
  testParser.cpp 
  testValidator.cpp 
  testWriter.cpp 
  testApiOtherActions.cpp 
  testApiAddingNode.cpp 
  testApiChangingNode.cpp 
  testApiRemovingNode.cpp 
  testApiError.cpp 
  ${READER}/Preparser.cpp 
  ${READER}/Parser.cpp 
  ${READER}/Validator.cpp 
  ${WRITER}/Writer.cpp 
  ${LOG}/Error.cpp 
  ${LOG}/ErrorStorage.cpp 
  ${SRC}/jsonApi.cpp 
  ${SRC}/keyMapper.cpp 
  ${SRC}/manager.cpp 
)


target_compile_features(jsonParserTest PRIVATE cxx_std_20)

target_include_directories(
  jsonParserTest 
  PRIVATE 
  googletest/include 
  ${HEADERS} 
  ${HEADERS_API}
)

target_link_libraries(
  jsonParserTest
  GTest::gtest_main  
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(jsonParserTest)

