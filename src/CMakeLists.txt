add_library(monar_core STATIC)

target_include_directories(monar_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/internal
)

target_compile_features(monar_core PUBLIC c_std_11)
monar_apply_common_warnings(monar_core)

if(MONAR_PLATFORM STREQUAL "stm32f407")
    monar_apply_embedded_compile_flags(monar_core)
endif()

add_subdirectory(device)
add_subdirectory(osal)
