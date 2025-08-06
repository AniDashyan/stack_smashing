cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulnerable main.cpp)
add_executable(protected main.cpp)

if(MSVC)
    target_compile_options(vulnerable PRIVATE /Od /GS-)
    target_compile_options(protected PRIVATE /Od /GS /guard:cf)
    target_link_options(protected PRIVATE /GUARD:CF)
else()
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)

    if(UNIX)
        # Allow execution from stack for vulnerable
        target_link_options(vulnerable PRIVATE -z execstack)
        # Disable execution from stack for protected
        target_link_options(protected PRIVATE -z noexecstack)
    endif()
endif()

enable_testing()

# Vulnerable test (must fail)
add_test(NAME vulnerable_run COMMAND vulnerable)
set_tests_properties(vulnerable_run PROPERTIES WILL_FAIL TRUE)

# Protected test (must pass)
add_test(NAME protected_run COMMAND protected)
set_tests_properties(vulnerable_run PROPERTIES WILL_FAIL TRUE)

add_custom_target(all_demos
    DEPENDS vulnerable protected
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running both demos via CTest"
)
