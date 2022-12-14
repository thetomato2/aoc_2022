cmake_minimum_required(VERSION 3.23)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/msvc_toolchain.cmake)

project ("AOC_2022"
        VERSION 0.1
        DESCRIPTION "Avent of Code 2022"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

file(GLOB INPUTS
    ${CMAKE_CURRENT_SOURCE_DIR}/inputs/*.txt
    )

add_definitions(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_DEPRECATE)

link_libraries(
    user32.lib
    # gdi32.lib
    winmm.lib
    kernel32.lib
    # Ole32.lib
    )

add_library(utils 
    src/utils.cc
    src/utils.hh
)

link_libraries(utils)

add_executable(day_01
    src/day_01.cc
)

add_executable(day_02
    src/day_02.cc
)

add_executable(day_03
    src/day_03.cc
)

add_executable(day_04
    src/day_04.cc
)

add_executable(day_05
    src/day_05.cc
)

add_executable(day_06
    src/day_06.cc
)

add_executable(day_07
    src/day_07.cc
)

add_executable(day_08
    src/day_08.cc
)

add_executable(day_09
    src/day_09.cc
)

add_executable(day_10
    src/day_10.cc
)

add_custom_target(copy)
add_custom_command(
    TARGET copy POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/inputs
    COMMAND ${CMAKE_COMMAND} -E copy
    ${INPUTS} 
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/inputs
    DEPENDS ${INPUTS}
)
