
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(HEADERS "../headers")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

configure_file(${PROJECT_SOURCE_DIR}/test/config.h.in ${PROJECT_SOURCE_DIR}/test/config.h)

enable_testing()

add_executable(
  jsoncpptests
  testPreparser.cpp
  testParser.cpp
  testValidator.cpp
  testWriter.cpp
  ../src/Preparser.cpp
  ../src/Parser.cpp
  ../src/ParserKey.cpp
  ../src/Validator.cpp
  ../src/Writer.cpp
 )

target_compile_features(jsoncpptests PRIVATE cxx_std_20)

target_include_directories(
  jsoncpptests 
  PRIVATE 
  googletest/include 
  ${HEADERS}
)

target_link_libraries(
  jsoncpptests
  GTest::gtest_main  
)

include(GoogleTest)
gtest_discover_tests(jsoncpptests)

