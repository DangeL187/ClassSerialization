cmake_minimum_required(VERSION 3.26)
project(ClassSerialization)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_library(ClassSerialization ${HEADER_FILES} ${SOURCE_FILES} ${TINYXML_SOURCE_FILE})
