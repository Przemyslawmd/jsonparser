
project(jsonParser)

set(HEADERS "../headers")
set(READER "reader/")
set(WRITER "writer/")

add_library(
    jsonParser
    SHARED
    jsonApi.cpp
    keyMapper.cpp
    utils.cpp
    ${READER}Parser.cpp
    ${READER}ParserKey.cpp
    ${READER}Preparser.cpp
    ${READER}Validator.cpp
    ${WRITER}Writer.cpp
    log/Error.cpp
  "manager.cpp")

target_include_directories(jsonParser PRIVATE ${HEADERS}) 

target_compile_features(jsonParser PRIVATE cxx_std_20)
