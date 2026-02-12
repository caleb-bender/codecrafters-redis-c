cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

project(codecrafters_redis_lib)

add_library(codecrafters_redis_lib
    src/lib.c
)

target_precompile_headers(
    codecrafters_redis_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include/pch.h
)

target_include_directories(
    codecrafters_redis_lib
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

project(codecrafters_redis)

add_executable(codecrafters_redis
    src/main.c
)

target_link_libraries(
    codecrafters_redis
    PRIVATE
    codecrafters_redis_lib
)

target_include_directories(
    codecrafters_redis
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

if(CMAKE_BUILD_UNIT_TESTS STREQUAL "ON")
    add_subdirectory(tests/unit)
endif()
