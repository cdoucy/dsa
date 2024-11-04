cmake_minimum_required(VERSION 3.0)
project(dsa)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TARGET dsa)

set (PROJECT_ROOT ./)
include(${PROJECT_ROOT}/src/src.cmake)
include(${PROJECT_ROOT}/inc/inc.cmake)

#https://github.com/fmtlib/fmt/archive/refs/tags/9.1.0.zip
include(FetchContent)
FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 9.1.0
)
FetchContent_MakeAvailable(fmt)

set(CMAKE_CXX_FLAGS "-std=c++20 -Wpedantic -Wall -Wextra ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g3 ${CMAKE_CXX_FLAGS_DEBUG}")

if (${CMAKE_BUILD_TYPE} STREQUAL "test")
    add_subdirectory(tests)
    return()
endif()

add_executable(
        ${TARGET}
        ${SOURCES}
        main.cpp
        src/Str.cpp
)

target_include_directories(
        ${TARGET}
        PUBLIC ${INCLUDES}
)

target_link_libraries(
        ${TARGET}
        fmt::fmt
)