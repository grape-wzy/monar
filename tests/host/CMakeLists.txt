add_executable(monar_host_test
    ${CMAKE_CURRENT_SOURCE_DIR}/test_device.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_osal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_main.c
)

target_link_libraries(monar_host_test PRIVATE monar_core)
target_include_directories(monar_host_test PRIVATE
    ${PROJECT_SOURCE_DIR}/src/internal
)

monar_apply_common_warnings(monar_host_test)

add_test(NAME monar_host_smoke COMMAND monar_host_test)
