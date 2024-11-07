set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

project(luc)

add_executable(lutest)
target_sources(lutest PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/main.c
)

target_include_directories(lutest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
