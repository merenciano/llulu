add_executable(mat4_test)
target_sources(mat4_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/mat4_test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/lu_hooks.c
)

target_include_directories(mat4_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(mat4_test PRIVATE
    m
)

add_test(NAME mat4_test COMMAND mat4_test)

add_executable(bench)
target_sources(bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/bench.c
)

target_include_directories(bench PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
