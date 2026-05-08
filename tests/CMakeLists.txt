set(PROJECT_NAME gtest-serialize-list-nodes)
project(${PROJECT_NAME})

# Projects dependencies.
set(PROJECT_LIBS)

if (BUILD_TESTING)
    set(GTEST_LIBS ${GTEST_LIBS} gtest)
else()
    set(GTEST_LIBS ${GTEST_LIBS} "")
endif(BUILD_TESTING)

# Include generated *.pb.h files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(../src)

# Collecting a list of sources.
file(GLOB SOURCE_FILES *.cpp ../src/common/*.cpp ../src/adapters/*.cpp ../src/builders/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS} ${GTEST_LIBS})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
