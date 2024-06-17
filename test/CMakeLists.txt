cmake_minimum_required(VERSION 3.26)
project(ClassSerializationTest)

set(CMAKE_CXX_STANDARD 23)

include_directories(../include)
link_directories(../build)

add_executable(ClassSerializationTest "main.cpp")
target_link_libraries(ClassSerializationTest ClassSerialization)
