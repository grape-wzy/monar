include_guard(GLOBAL)

set(MONAR_PLATFORM "host" CACHE STRING
    "Monar build platform: host or stm32f407")
set_property(CACHE MONAR_PLATFORM PROPERTY STRINGS host stm32f407)

set(MONAR_RUNTIME "baremetal" CACHE STRING
    "Monar runtime backend: baremetal")
set_property(CACHE MONAR_RUNTIME PROPERTY STRINGS baremetal)

option(MONAR_BUILD_TESTS "Build Monar test targets" ON)
option(MONAR_BUILD_EXAMPLES "Build Monar example targets" ON)

function(monar_apply_common_warnings target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
        )
    endif()
endfunction()

function(monar_apply_embedded_compile_flags target)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -ffreestanding
            -fdata-sections
            -ffunction-sections
        )
    endif()
endfunction()

function(monar_apply_stm32f407_link_options target linker_script)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${target} PRIVATE
            -mcpu=cortex-m4
            -mthumb
            -nostartfiles
            -nostdlib
            -Wl,--gc-sections
            -Wl,--build-id=none
            -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${target}.map
            -T${linker_script}
        )
    endif()
endfunction()
