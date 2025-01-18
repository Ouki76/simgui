cmake_minimum_required(VERSION 3.12)
project(simgui)

set(CMAKE_CXX_STANDARD 26)

add_executable(${PROJECT_NAME} examples/main.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_dx11.cpp
    imgui/backends/imgui_impl_win32.cpp
)

include_directories(imgui)