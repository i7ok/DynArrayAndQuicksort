cmake_minimum_required(VERSION 3.20)
project(labs_p21 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/utf-8)
endif()

add_library(labs_headers INTERFACE)
target_include_directories(labs_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
  tests/test_array.cpp
  tests/test_sort.cpp
)
target_link_libraries(tests PRIVATE labs_headers GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

add_executable(bench_sort
  bench/bench_sort.cpp
)
target_link_libraries(bench_sort PRIVATE labs_headers)

add_executable(file_sort
  app/file_sort.cpp
)
target_link_libraries(file_sort PRIVATE labs_headers)
