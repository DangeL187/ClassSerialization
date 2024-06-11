cmake_minimum_required(VERSION 3.26)
project(ClassSerialization)

set(CMAKE_CXX_STANDARD 23)

# set(DEPS_FOLDER D:/DEPS)

#if (WIN32)
#    set(MANUAL_BOOST ON)
#endif ()
#if (UNIX)
#    set(MANUAL_BOOST OFF)
#endif ()
#
#if (MANUAL_BOOST)
#    set(BOOST_ROOT ${DEPS_FOLDER}/boost) # SETTING PATH TO THE BOOST LIB
#    include_directories(${BOOST_ROOT})
#    link_directories(${BOOST_ROOT}/stage/lib)
#    set(BOOST_LIBS boost_system-mgw13-mt-d-x64-1_84 boost_filesystem-mgw13-mt-d-x64-1_84 boost_serialization-mgw13-mt-d-x64-1_84)
#else()
#    find_package(Boost REQUIRED)
#    set(BOOST_LIBS Boost::boost)
#endif ()

include_directories(include)
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_executable(ClassSerialization ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(ClassSerialization)
