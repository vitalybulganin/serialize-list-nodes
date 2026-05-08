cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME serialize-list-nodes)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTING "Building unit-tests" ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif (BUILD_TESTING)
