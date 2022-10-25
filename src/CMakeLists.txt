cmake_minimum_required(VERSION 3.23)
project(flow_plugin_sdk_src)
SET(PROJECT_NAME flow_plugin_sdk_src)

set(CMAKE_CXX_STANDARD 20)

set(src_flight_information_regions
        flightinformationregion/FlightInformationRegion.h
        flightinformationregion/ConcreteFlightInformationRegion.h flightinformationregion/ConcreteFlightInformationRegion.cpp)

set(src_events
        event/Event.h
        event/ConcreteEvent.cpp event/ConcreteEvent.h)

set(src_pch
        pch.h pch.cpp)

set(ALL_FILES
        ${src_events}
        ${src_flight_information_regions}
        ${src_pch}
    )

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
