cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project("ivo_sandbox")

set(CMAKE_CXX_STANDARD 20)

option(TEST_OPTION "Test option" OFF)

if (TEST_OPTION)
    add_definitions(-DTEST_OPTION)
endif()

add_executable (ivo_sandbox main.cpp)
target_link_libraries(ivo_sandbox mysqlclient dl)