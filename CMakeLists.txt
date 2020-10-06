cmake_minimum_required(VERSION 3.2)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(uv-sanatizer)

add_executable(uv-sanatizer uv-sanatizer.cpp)

target_link_arduino_libraries(uv-sanatizer PRIVATE core)
target_enable_arduino_upload(uv-sanatizer)
