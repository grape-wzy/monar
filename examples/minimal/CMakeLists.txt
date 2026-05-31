get_property(monar_bsp_sources GLOBAL PROPERTY MONAR_ACTIVE_BSP_STARTUP_SOURCES)
get_property(monar_bsp_include_dirs GLOBAL PROPERTY MONAR_ACTIVE_BSP_INCLUDE_DIRS)
get_property(monar_bsp_compile_definitions GLOBAL PROPERTY
    MONAR_ACTIVE_BSP_COMPILE_DEFINITIONS)
get_property(monar_bsp_linker_script GLOBAL PROPERTY MONAR_ACTIVE_BSP_LINKER_SCRIPT)

if(NOT monar_bsp_sources OR NOT monar_bsp_linker_script)
    message(FATAL_ERROR
        "MONAR_PLATFORM='${MONAR_PLATFORM}' requires BSP sources before "
        "building examples/minimal")
endif()

add_executable(monar_stm32f407_minimal.elf
    ${CMAKE_CURRENT_SOURCE_DIR}/main.c
    ${monar_bsp_sources}
)

target_link_libraries(monar_stm32f407_minimal.elf PRIVATE monar_core)
target_include_directories(monar_stm32f407_minimal.elf PRIVATE
    ${monar_bsp_include_dirs}
)
target_compile_definitions(monar_stm32f407_minimal.elf PRIVATE
    ${monar_bsp_compile_definitions}
)

monar_apply_embedded_compile_flags(monar_stm32f407_minimal.elf)
monar_apply_stm32f407_link_options(monar_stm32f407_minimal.elf
    ${monar_bsp_linker_script})

if(CMAKE_OBJCOPY)
    add_custom_command(TARGET monar_stm32f407_minimal.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex
            $<TARGET_FILE:monar_stm32f407_minimal.elf>
            ${CMAKE_CURRENT_BINARY_DIR}/monar_stm32f407_minimal.hex
        COMMAND ${CMAKE_OBJCOPY} -O binary
            $<TARGET_FILE:monar_stm32f407_minimal.elf>
            ${CMAKE_CURRENT_BINARY_DIR}/monar_stm32f407_minimal.bin
        VERBATIM
    )
endif()

if(MONAR_SIZE_UTIL)
    add_custom_command(TARGET monar_stm32f407_minimal.elf POST_BUILD
        COMMAND ${MONAR_SIZE_UTIL} $<TARGET_FILE:monar_stm32f407_minimal.elf>
        VERBATIM
    )
endif()
