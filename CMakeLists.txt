cmake_minimum_required(VERSION 3.20)
project(loxpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

option(LOXPP_BUILD_TESTS "Build unit tests" ON)
option(LOXPP_ENABLE_WARNINGS_AS_ERRORS ON)

add_library(loxpp_warnings INTERFACE)
target_compile_options(loxpp_warnings INTERFACE
    $<$<CXX_COMPILER_ID:GNU,Clang>:
      -Wall -Wextra -Wpedantic -Wshadow -Wconversion
      -Wsign-conversion
      -Wold-style-cast
      -Wcast-align
      -Wswitch-enum
      -Wunused
      -Woverloaded-virtual
      -Wnon-virtual-dtor
      -Wnull-dereference
      -Wdouble-promotion
      -Wformat=2
      -Wimplicit-fallthrough>
    $<$<CXX_COMPILER_ID:GNU>:
      -Wlogical-op
      -Wuseless-cast
      -Wduplicated-cond
      -Wduplicated-branches
      -Wmisleading-indentation>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
if(LOXPP_ENABLE_WARNINGS_AS_ERRORS)
    target_compile_options(loxpp_warnings INTERFACE
        $<$<CXX_COMPILER_ID:GNU,Clang>:-Werror>
        $<$<CXX_COMPILER_ID:MSVC>:/WX>
    )
endif()

add_library(loxpp_core
    src/common.h
    src/chunk.h
    src/chunk.cpp
    src/memory.h
    src/debug.cpp
    src/debug.h
    src/value.h
    src/value.cpp
    src/line.h
    src/line.cpp
    src/vm.cpp
    src/vm.h
    src/opcode.h
    src/app.h
    src/app.cpp
    src/fs.cpp
    src/fs.h
)
add_library(loxpp::core ALIAS loxpp_core)

target_include_directories(loxpp_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(loxpp_core PRIVATE loxpp_warnings)

add_executable(loxpp src/main.cpp)
target_link_libraries(loxpp PRIVATE loxpp::core loxpp_warnings)

include(FetchContent)

if(LOXPP_BUILD_TESTS)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.7.1
    )
    FetchContent_MakeAvailable(Catch2)

    enable_testing()
    add_subdirectory(tests)
endif()
