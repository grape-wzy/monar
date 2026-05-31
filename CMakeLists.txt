cmake_minimum_required(VERSION 3.20)

include(cmake/monar_options.cmake)

project(Monar VERSION 0.1.0 LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MONAR_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(silicon)
add_subdirectory(bsp)
add_subdirectory(src)
add_subdirectory(runtimes)

if(MONAR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(MONAR_BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(monar_sync_compile_commands
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
        COMMENT "Sync compile_commands.json to repository root"
        VERBATIM
    )
endif()
