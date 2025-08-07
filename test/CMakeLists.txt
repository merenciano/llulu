add_executable(bench_str)
target_sources(bench_str PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bench_str.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_hooks.c
)

target_include_directories(bench_str PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_compile_definitions(bench_str PRIVATE
    LU_DISABLE_ASSERTS
    LU_DISABLE_CONTRACTS
)

target_link_libraries(bench_str PRIVATE
    m
)

add_executable(test_data_types)
target_sources(test_data_types PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test_data_types.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_time.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_log.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_hooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_buffer.c
)

target_include_directories(test_data_types PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

add_test(NAME test_data_types COMMAND test_data_types)
