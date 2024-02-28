cmake_minimum_required(VERSION 3.13)

project(scheduler LANGUAGES C)

add_library(scheduler scheduler.c)
target_include_directories(scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(scheduler PROPERTIES C_STANDARD 90)

target_compile_options(scheduler PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
        -Wall
        -Wextra
    >
    $<$<C_COMPILER_ID:MSVC>:
        /W4
    >
)
