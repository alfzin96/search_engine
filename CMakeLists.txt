cmake_minimum_required(VERSION 3.28)

project(search_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(tests)

include(FetchContent)


FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_disable_pthreads on)
FetchContent_MakeAvailable(nlohmann_json googletest)

enable_testing()

add_executable(${PROJECT_NAME} src/main.cpp src/converter_json.h src/converter_json.cpp src/inverted_index.h src/inverted_index.cpp src/search_server.h src/search_server.cpp include/my_exceptions.h)
target_include_directories(${PROJECT_NAME} PUBLIC include src)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)






