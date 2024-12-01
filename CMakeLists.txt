set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

project(llulu)

include_directories(./include)

add_executable(unitt)
target_sources(unitt PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/unitt.c
)

add_executable(fuzzt)
target_sources(fuzzt PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/fuzzt.c
)

add_executable(bench)
target_sources(bench PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/bench.c
)
