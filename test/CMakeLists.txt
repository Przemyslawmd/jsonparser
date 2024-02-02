
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(HEADERS "../headers")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  jsoncpptests
  "testPreparser.cpp"
  "../src/Preparser.cpp"
  )

target_compile_features(jsoncpptests PRIVATE cxx_std_17)

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

