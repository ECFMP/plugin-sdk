cmake_minimum_required(VERSION 3.23)
project(ecfmp_plugin_sdk)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

##### Configuration Types
set(CMAKE_CONFIGURATION_TYPES
        "Debug"
        "Release"
        CACHE STRING "" FORCE
        )

add_subdirectory(src)
add_subdirectory(test)
