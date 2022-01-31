cmake_minimum_required(VERSION 3.18)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(FetchContent)

project(LvglPico)

pico_sdk_init()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


FetchContent_Declare(lvgl URL https://github.com/lvgl/lvgl.git)
FetchContent_MakeAvailable(lvgl)

# target_link_libraries(xxx PRIVATE lvgl::lvgl)

