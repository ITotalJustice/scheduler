cmake_minimum_required(VERSION 3.13)

project(scheduler
    VERSION 1.0.0
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

add_subdirectory(src)
