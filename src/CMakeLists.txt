
project(jsonParser)

set(HEADERS "headers/")
set(HEADERS_API "../headers/")
set(LOG "log")
set(READER "reader")
set(WRITER "writer")

add_library(
    jsonParser 
    SHARED 
    ${READER}/Parser.cpp 
    ${READER}/Preparser.cpp 
    ${READER}/Validator.cpp 
    ${WRITER}/Writer.cpp 
    ${LOG}/Error.cpp 
    ${LOG}/ErrorStorage.cpp 
    jsonApi.cpp 
    keyMapper.cpp 
    manager.cpp 
)

target_include_directories(
    jsonParser 
    PRIVATE 
    ${HEADERS} 
    ${HEADERS_API}
)

target_compile_features(jsonParser PRIVATE cxx_std_20)
