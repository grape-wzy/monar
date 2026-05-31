cmake_minimum_required(VERSION 3.20)

project(Monar VERSION 0.1.0 LANGUAGES C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

set(MONAR_PLATFORM "host" CACHE STRING
    "Monar build platform: host or stm32f407")
set_property(CACHE MONAR_PLATFORM PROPERTY STRINGS host stm32f407)

set(MONAR_COMMON_SOURCES
    src/osal/mn_osal.c
    src/device/mn_device.c
    src/device/mn_device_registry.c
    runtimes/baremetal/mn_runtime_baremetal.c
)

add_library(monar_core STATIC ${MONAR_COMMON_SOURCES})
target_include_directories(monar_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
)
target_compile_features(monar_core PUBLIC c_std_11)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(monar_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CMAKE_EXPORT_COMPILE_COMMANDS)
    add_custom_target(monar_sync_compile_commands ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
        COMMENT "Sync compile_commands.json to repository root"
        VERBATIM
    )
endif()

if(MONAR_PLATFORM STREQUAL "host")
    add_executable(monar_host_test tests/host/test_main.c)
    target_link_libraries(monar_host_test PRIVATE monar_core)
    target_include_directories(monar_host_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
    )
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(monar_core PRIVATE -Werror)
        target_compile_options(monar_host_test PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
        )
    endif()
    add_test(NAME monar_host_smoke COMMAND monar_host_test)
elseif(MONAR_PLATFORM STREQUAL "stm32f407")
    set(MONAR_STM32_BOARD_ROOT
        ${CMAKE_CURRENT_SOURCE_DIR}/bsp/boards/st/nucleo_f407zg)
    set(MONAR_STM32_LINKER_SCRIPT
        ${MONAR_STM32_BOARD_ROOT}/linker/stm32f407zg_flash.ld)

    add_executable(monar_stm32f407_minimal.elf
        ${MONAR_STM32_BOARD_ROOT}/startup/startup_stm32f407xx.c
        ${MONAR_STM32_BOARD_ROOT}/board_init.c
        examples/minimal/main.c
    )

    target_link_libraries(monar_stm32f407_minimal.elf PRIVATE monar_core)
    target_include_directories(monar_stm32f407_minimal.elf PRIVATE
        ${MONAR_STM32_BOARD_ROOT}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/internal
    )
    target_compile_definitions(monar_stm32f407_minimal.elf PRIVATE
        STM32F407xx
    )

    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(monar_core PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -ffreestanding
            -fdata-sections
            -ffunction-sections
        )
        target_compile_options(monar_stm32f407_minimal.elf PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -ffreestanding
            -fdata-sections
            -ffunction-sections
        )
        target_link_options(monar_stm32f407_minimal.elf PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -nostartfiles
            -nostdlib
            -Wl,--gc-sections
            -Wl,--build-id=none
            -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/monar_stm32f407_minimal.map
            -T${MONAR_STM32_LINKER_SCRIPT}
        )
    endif()

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
else()
    message(FATAL_ERROR "Unsupported MONAR_PLATFORM='${MONAR_PLATFORM}'")
endif()
