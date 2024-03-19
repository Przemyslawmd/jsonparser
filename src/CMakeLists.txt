
project(jsoncppapp)

set(HEADERS "../headers")

add_executable(jsoncppapp main.cpp)

target_include_directories(jsoncppapp PUBLIC ${HEADERS}) 

target_compile_features(jsoncppapp PRIVATE cxx_std_20)
