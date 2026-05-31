set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(MONAR_ARM_GCC_ROOT "$ENV{MONAR_ARM_GCC_ROOT}" CACHE PATH
    "Root directory of the Arm GNU Toolchain")

if(NOT MONAR_ARM_GCC_ROOT)
    set(_mn_arm_gcc_candidates)
    file(GLOB _mn_arm_gcc_candidates
        "C:/Program Files/Arm GNU Toolchain arm-none-eabi/*"
        "C:/Program Files (x86)/Arm GNU Toolchain arm-none-eabi/*"
        "C:/My Program/Arm GNU Toolchain arm-none-eabi/*"
    )
    list(SORT _mn_arm_gcc_candidates ORDER DESCENDING)
    list(LENGTH _mn_arm_gcc_candidates _mn_arm_gcc_candidate_count)
    if(_mn_arm_gcc_candidate_count GREATER 0)
        list(GET _mn_arm_gcc_candidates 0 MONAR_ARM_GCC_ROOT)
    endif()
endif()

find_program(CMAKE_C_COMPILER
    NAMES arm-none-eabi-gcc
    HINTS ${MONAR_ARM_GCC_ROOT}
    PATH_SUFFIXES bin
    REQUIRED
)

find_program(CMAKE_ASM_COMPILER
    NAMES arm-none-eabi-gcc
    HINTS ${MONAR_ARM_GCC_ROOT}
    PATH_SUFFIXES bin
    REQUIRED
)

find_program(CMAKE_AR
    NAMES arm-none-eabi-ar
    HINTS ${MONAR_ARM_GCC_ROOT}
    PATH_SUFFIXES bin
)

find_program(CMAKE_OBJCOPY
    NAMES arm-none-eabi-objcopy
    HINTS ${MONAR_ARM_GCC_ROOT}
    PATH_SUFFIXES bin
)

find_program(MONAR_SIZE_UTIL
    NAMES arm-none-eabi-size
    HINTS ${MONAR_ARM_GCC_ROOT}
    PATH_SUFFIXES bin
)

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
