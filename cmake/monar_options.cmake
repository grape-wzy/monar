include_guard(GLOBAL)

set(MONAR_PLATFORM "host" CACHE STRING
    "Monar build platform: host or stm32f407")
set_property(CACHE MONAR_PLATFORM PROPERTY STRINGS host stm32f407)

set(MONAR_RUNTIME "baremetal" CACHE STRING
    "Monar runtime backend: baremetal")
set_property(CACHE MONAR_RUNTIME PROPERTY STRINGS baremetal)

option(MONAR_BUILD_TESTS "Build Monar test targets" ON)
option(MONAR_BUILD_EXAMPLES "Build Monar example targets" ON)

function(monar_is_arm_embedded_compiler out_var)
    if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
        if(CMAKE_C_COMPILER MATCHES "arm-none-eabi")
            set(${out_var} TRUE PARENT_SCOPE)
            return()
        endif()
    endif()

    if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_C_COMPILER_TARGET MATCHES "^arm.*eabi" OR
                CMAKE_C_COMPILER MATCHES "arm-none-eabi")
            set(${out_var} TRUE PARENT_SCOPE)
            return()
        endif()
    endif()

    set(${out_var} FALSE PARENT_SCOPE)
endfunction()

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
    monar_is_arm_embedded_compiler(monar_has_arm_embedded_compiler)
    if(monar_has_arm_embedded_compiler)
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
    monar_is_arm_embedded_compiler(monar_has_arm_embedded_compiler)
    if(monar_has_arm_embedded_compiler)
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
