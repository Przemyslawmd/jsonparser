
project(jsonParser)

add_library(
    jsonParser
    SHARED
    reader/Parser.cpp
    reader/Preparser.cpp
    reader/Validator.cpp
    writer/Writer.cpp
    log/Error.cpp
    log/ErrorStorage.cpp
    jsonApi.cpp
    keyMapper.cpp
    manager.cpp
)

target_include_directories(
    jsonParser
    PRIVATE
    "headers/"
    "../headers/"
)

target_compile_features(jsonParser PRIVATE cxx_std_20)
