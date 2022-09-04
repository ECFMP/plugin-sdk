cmake_minimum_required(VERSION 3.23)
project(flow_plugin_sdk)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)
