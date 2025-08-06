cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

# no stack protection
add_executable(vulnerable main.cpp)

# with stack protection
add_executable(protected main.cpp)

if(MSVC)
    target_compile_options(vulnerable PRIVATE /Od /GS-)
    target_compile_options(protected PRIVATE /Od /GS /guard:cf)
    target_link_options(protected PRIVATE /GUARD:CF)
else()
    # GCC/Clang flags
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)
    

    if(UNIX)
        target_link_options(vulnerable PRIVATE -z execstack)
        target_link_options(protected PRIVATE -z noexecstack)
    endif()
endif()

add_custom_target(all_demos 
    DEPENDS vulnerable protected
    COMMAND ${CMAKE_COMMAND} -E echo "Running vulnerable version:"
    COMMAND $<TARGET_FILE:vulnerable>
    COMMAND ${CMAKE_COMMAND} -E echo "Running protected version:"
    COMMAND $<TARGET_FILE:protected>
    COMMENT "Building and running both demos"
)