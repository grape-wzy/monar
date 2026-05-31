add_library(monar_core STATIC)

get_property(monar_silicon_sources GLOBAL PROPERTY MONAR_ACTIVE_SILICON_SOURCES)
get_property(monar_silicon_include_dirs GLOBAL PROPERTY
    MONAR_ACTIVE_SILICON_INCLUDE_DIRS)
get_property(monar_silicon_compile_definitions GLOBAL PROPERTY
    MONAR_ACTIVE_SILICON_COMPILE_DEFINITIONS)
get_property(monar_bsp_core_sources GLOBAL PROPERTY MONAR_ACTIVE_BSP_CORE_SOURCES)
get_property(monar_bsp_include_dirs GLOBAL PROPERTY MONAR_ACTIVE_BSP_INCLUDE_DIRS)
get_property(monar_bsp_compile_definitions GLOBAL PROPERTY
    MONAR_ACTIVE_BSP_COMPILE_DEFINITIONS)

if(monar_silicon_sources)
    target_sources(monar_core PRIVATE ${monar_silicon_sources})
endif()

if(monar_bsp_core_sources)
    target_sources(monar_core PRIVATE ${monar_bsp_core_sources})
endif()

target_include_directories(monar_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/internal
        ${monar_silicon_include_dirs}
        ${monar_bsp_include_dirs}
)

target_compile_definitions(monar_core PRIVATE
    ${monar_silicon_compile_definitions}
    ${monar_bsp_compile_definitions}
)

target_compile_features(monar_core PUBLIC c_std_11)
monar_apply_common_warnings(monar_core)

if(MONAR_PLATFORM STREQUAL "stm32f407")
    monar_apply_embedded_compile_flags(monar_core)
endif()

add_subdirectory(device)
add_subdirectory(core)
add_subdirectory(osal)
