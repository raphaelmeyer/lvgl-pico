cmake_minimum_required(VERSION 3.18)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(LvglPico)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(helloworld)

target_sources(helloworld PRIVATE
  main.cc
)

target_link_libraries(helloworld PRIVATE
  pico_stdlib
  lvgl::lvgl
)

target_compile_options(helloworld PRIVATE -Wall -Wextra -Werror -Wno-ignored-qualifiers)

pico_add_extra_outputs(helloworld)





add_subdirectory(lvgl)
