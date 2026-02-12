
project(codecrafters_redis_unit_tests)

add_executable(
    codecrafters_redis_unit_tests
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_precompile_headers(
    codecrafters_redis_unit_tests
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h
)

target_link_libraries(
    codecrafters_redis_unit_tests
    PRIVATE
    codecrafters_redis_lib
)

target_include_directories(
    codecrafters_redis_unit_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
    codecrafters_redis_unit_tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../vendor
)