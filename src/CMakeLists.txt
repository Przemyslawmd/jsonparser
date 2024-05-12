
project(jsonParserLib)

set(HEADERS "../headers")

add_library(
    jsonParserLib 
    STATIC
    jsonApi.cpp)

target_include_directories(jsonParserLib PUBLIC ${HEADERS}) 

target_compile_features(jsonParserLib PRIVATE cxx_std_20)
