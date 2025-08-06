cmake_minimum_required(VERSION 3.10)
project(stack_smashing)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulnerable vulnerable.cpp)
add_executable(protected protected.cpp)

if(MSVC)
    target_compile_options(vulnerable PRIVATE /Od /GS-)
    target_compile_options(protected PRIVATE /Od /GS /guard:cf)
    target_link_options(protected PRIVATE /GUARD:CF)
else()
    target_compile_options(vulnerable PRIVATE -O0 -fno-stack-protector)
    target_compile_options(protected PRIVATE -O0 -fstack-protector-all)
    if(UNIX)
        target_link_options(vulnerable PRIVATE -z execstack)
        target_link_options(protected PRIVATE -z noexecstack)
    endif()
endif()

if (MSVC) 
    add_custom_command(
        TARGET all_demos
        COMMAND cmd /C "timeout /T 10 /NOBREAK >nul & $<TARGET_FILE:vulnerable> || echo Vulnerable terminated"
        COMMAND cmd /C "timeout /T 10 /NOBREAK >nul & $<TARGET_FILE:protected> || echo Protected terminated"
        COMMENT "Running executables with timeout on Windows (MSVC)"
    )
else()
    add_custom_target(all_demos
        COMMAND $<TARGET_FILE:vulnerable> || echo "Vulnerable terminated"
        COMMAND $<TARGET_FILE:protected> || echo "Protected terminated"
        DEPENDS vulnerable protected
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif()
