cmake_minimum_required(VERSION 3.10.2)

project(s21Containers)
include(FetchContent)
FetchContent_Declare(
    googletest

    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0 -std=c++17 -Wall -Werror -Wextra")

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# add_library(
# s21_containers

# # source/list/s21_list.h
# source/s21_containers.h
# )
add_executable(
    s21_containers_tests
    tests/t_lists.cpp
    tests/test.h
)

target_link_libraries(
    s21_containers_tests

    # s21_containers
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(s21_containers_tests)
