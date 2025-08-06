cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulnerable vulnerable.cpp)
add_executable(protected protected.cpp)

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

add_custom_target(all_demos
    DEPENDS vulnerable protected
    COMMAND $<TARGET_FILE:vulnerable> || echo "Vulnerable terminated"
    COMMAND $<TARGET_FILE:protected> || echo "Protected terminated"
    COMMENT "Running both executables"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)