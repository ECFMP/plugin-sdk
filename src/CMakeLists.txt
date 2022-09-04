cmake_minimum_required(VERSION 3.23)
project(flow_plugin_sdk_src)

set(CMAKE_CXX_STANDARD 20)

set(ALL_FILES
        true.cpp
        )

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
