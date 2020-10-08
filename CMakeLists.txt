cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uv-sanatizer)

add_executable(uv-sanatizer
uv-sanatizer.cpp
debounce_task.cpp
debounce_task.hpp
sanatize_task.cpp
sanatize_task.hpp
rising_edge_task.cpp
rising_edge_task.hpp
)

target_link_arduino_libraries(uv-sanatizer PRIVATE core)
target_enable_arduino_upload(uv-sanatizer)