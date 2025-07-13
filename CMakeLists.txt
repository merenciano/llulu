set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

project(llulu)

include(CTest)

include_directories(./include)

add_subdirectory(test)
