
project(jsonParserLib)

set(HEADERS "../headers")
set(READER "reader/")
set(WRITER "writer/")

add_library(
    jsonParserLib 
    STATIC
    jsonApi.cpp
    ${READER}Parser.cpp
    ${READER}ParserKey.cpp
    ${READER}Preparser.cpp
    ${READER}Validator.cpp
    ${WRITER}Writer.cpp)

target_include_directories(jsonParserLib PUBLIC ${HEADERS}) 

target_compile_features(jsonParserLib PRIVATE cxx_std_20)
