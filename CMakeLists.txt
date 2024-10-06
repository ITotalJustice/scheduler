cmake_minimum_required(VERSION 3.13)

project(scheduler
    VERSION 1.1.0
    LANGUAGES C
)

# export compile commands
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# enable testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    if (BUILD_TESTING)
        add_subdirectory(tests)
    endif()
endif()

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
