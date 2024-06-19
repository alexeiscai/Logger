cmake_minimum_required(VERSION 3.10)
project(logger)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(logger 
                src/main.cpp
                src/Logger.cpp)

target_include_directories(logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)



# Gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(Logger STATIC src/Logger.cpp)

target_include_directories(Logger PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

add_executable(tests tests/test_Logger.cpp)

target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

target_link_libraries(tests PRIVATE Logger ${GTEST_LIBRARIES} GTest::Main pthread)

add_custom_target(testss COMMAND tests DEPENDS tests)