cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulnerable main.cpp)
add_executable(protected main.cpp)

if(MSVC)
    #  no stack protection
    target_compile_options(vulnerable PRIVATE /Od /GS-)
    #  with stack protection
    target_compile_options(protected PRIVATE /Od /GS /guard:cf)
    target_link_options(protected PRIVATE /GUARD:CF)
else()
    #  no stack protector
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    # with stack protection
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)

    if(UNIX)
        # Allow execution from stack for vulnerable
        target_link_options(vulnerable PRIVATE -z execstack)
        # Disable execution from stack for protected
        target_link_options(protected PRIVATE -z noexecstack)
    endif()
endif()

if(WIN32)
    # cmd.exe ignores failure with "|| exit /b 0"
    set(RUN_IGNORE_FAIL cmd /C "$<TARGET_FILE:vulnerable> || exit /b 0")
else()
    # bash ignores failure with "|| true"
    set(RUN_IGNORE_FAIL bash -c "$<TARGET_FILE:vulnerable> || true")
endif()


add_custom_target(all_demos
    DEPENDS vulnerable protected
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running vulnerable version (expected to crash) ==="
    COMMAND ${RUN_IGNORE_FAIL}
    COMMAND ${CMAKE_COMMAND} -E echo "=== Running protected version (should not crash) ==="
    COMMAND $<TARGET_FILE:protected>
    COMMENT "Building and running both demos"
)