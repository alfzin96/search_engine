cmake_minimum_required(VERSION 3.28)

add_executable(mytest test.cpp ${CMAKE_SOURCE_DIR}/src/inverted_index.h ${CMAKE_SOURCE_DIR}/src/inverted_index.cpp ${CMAKE_SOURCE_DIR}/src/search_server.h ${CMAKE_SOURCE_DIR}/src/search_server.cpp)
target_include_directories(mytest PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(mytest PRIVATE gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(mytest)