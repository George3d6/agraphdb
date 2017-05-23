cmake_minimum_required(VERSION 3.7)

project(graphdb)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_BUILD_TYPE=Debug")
SET(CMAKE_CXX_FLAGS "--std=c++17")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g2")

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/*.h
    ${PROJECT_SOURCE_DIR}/*cpp
    ${PROJECT_SOURCE_DIR}/unit_tests/*.h
    ${PROJECT_SOURCE_DIR}/unit_tests/*cpp
    ${PROJECT_SOURCE_DIR}/functional_tests/*.h
    ${PROJECT_SOURCE_DIR}/functional_tests/*cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
