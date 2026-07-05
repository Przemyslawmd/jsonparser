
add_library(
    jsonParser
    SHARED
    "reader/json/parser.cpp"
    "reader/json/preparser.cpp"
    "writer/json/writer.cpp"
    "log/Error.cpp"
    "log/ErrorStorage.cpp"
    "jsonApi.cpp"
    "keyMapper.cpp"
    "manager.cpp"
)

target_include_directories(
    jsonParser
    PRIVATE
    "headers/"
    "../headers/"
    "./"
)

target_compile_features(jsonParser PRIVATE cxx_std_20)
