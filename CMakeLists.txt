cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulnerable main.cpp)
add_executable(protected main.cpp)

if(MSVC)
    target_compile_options(vulnerable PRIVATE /Od /GS-)
    target_compile_options(protected PRIVATE /Od /GS /guard:cf)
    target_link_options(protected PRIVATE /GUARD:CF)
elseif(MINGW)
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)
    target_link_options(vulnerable PRIVATE -Wl,--nxcompat -Wl,--dynamicbase)
    target_link_options(protected PRIVATE -Wl,--nxcompat -Wl,--dynamicbase)
else()
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)
    if(UNIX)
        target_link_options(vulnerable PRIVATE -z execstack)
        target_link_options(protected PRIVATE -z noexecstack)
    endif()
endif()

enable_testing()

add_test(NAME vulnerable_run COMMAND vulnerable)
set_tests_properties(vulnerable_run PROPERTIES WILL_FAIL TRUE)

add_test(NAME protected_run COMMAND protected)

# Option 1: Simple custom target that just builds both executables
add_custom_target(build_demos
    DEPENDS vulnerable protected
    COMMENT "Building both demo executables"
)

# Option 2: Custom target that runs executables directly (recommended)
add_custom_target(run_demos
    DEPENDS vulnerable protected
    COMMAND echo "=== Running vulnerable version (expected to crash) ==="
    COMMAND vulnerable || echo "Vulnerable version crashed as expected"
    COMMAND echo "=== Running protected version (should complete safely) ==="
    COMMAND protected
    COMMENT "Running both demos directly"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Option 3: If you still want to use CTest, run tests individually
add_custom_target(test_demos
    DEPENDS vulnerable protected
    COMMAND echo "Running individual tests..."
    COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_CURRENT_BINARY_DIR} -R protected_run --output-on-failure || true
    COMMAND echo "Note: vulnerable test is expected to fail and is not run in this demo"
    COMMENT "Running tests selectively"
)