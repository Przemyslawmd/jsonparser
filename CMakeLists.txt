
cmake_minimum_required(VERSION 3.20)
project(jsonParserProject)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 10 OR GCC_VERSION VERSION_EQUAL 10))
        message(FATAL_ERROR "It requires gcc version 10 or greater.")
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(src)
add_subdirectory(test)
