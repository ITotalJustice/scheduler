cmake_minimum_required(VERSION 3.13)

project(tests LANGUAGES C)

macro(create_test name)
    add_executable(test_${name} test_${name}.c)
    target_link_libraries(test_${name} PRIVATE scheduler)

    add_test(
        NAME test_${name}
        COMMAND test_${name}
    )
endmacro()

create_test(tick)
create_test(cycles_late)
create_test(custom_reset_event)
