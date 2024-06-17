cmake_minimum_required(VERSION 3.30 FATAL_ERROR)

project(dicer VERSION 0.1.0)

# --- Project Dependencies ---

# Setup dependency management tool
file(DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.0/CPM.cmake
     ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
     EXPECTED_HASH SHA256=7b354f3a5976c4626c876850c93944e52c83ec59a159ae5de5be7983f0e17a2a
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

# Add dev dependencies
CPMAddPackage("gh:doctest/doctest@2.4.11")

# --- Dicer library ---

add_library(${PROJECT_NAME})

file(GLOB_RECURSE headers CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp"
)
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${headers} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_26)
target_compile_options(
  ${PROJECT_NAME} PRIVATE -freflection -Weverything -Werror -Wno-c++98-compat
                          -Wno-c++98-compat-pedantic -Wno-padded
)

# --- Testing ---

enable_testing()

set(PROJECT_TEST_NAME ${PROJECT_NAME}_test)
add_executable(${PROJECT_TEST_NAME})

file(GLOB test_sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")
target_sources(${PROJECT_TEST_NAME} PRIVATE ${test_sources})

target_compile_features(${PROJECT_TEST_NAME} PRIVATE cxx_std_26)
target_compile_options(
  ${PROJECT_TEST_NAME}
  PRIVATE -freflection
          -Weverything
          -Werror
          -Wno-c++98-compat
          -Wno-c++98-compat-pedantic
          -Wno-padded
          -Wno-undefined-func-template
)
target_link_libraries(${PROJECT_TEST_NAME} doctest::doctest ${PROJECT_NAME})

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_TEST_NAME})
