cmake_minimum_required(VERSION 3.23)
project(flow_plugin_sdk_tests)
SET(PROJECT_NAME flow_plugin_sdk_tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

#### GOOGLE TEST SETUP
include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/../third_party/googletest" "third_party/googletest")

#### FILES IN THIS PROJECT AND EXECUTABLE
set(ALL_FILES
        main.cpp
        event/ConcreteEventTest.cpp
        event/ConcreteEventParticipantTest.cpp
        flightinformationregion/ConcreteFlightInformationRegionTest.cpp flowmeasure/ConcreteAirportFilterTest.cpp flowmeasure/ConcreteEventFilterTest.cpp flowmeasure/ConcreteLevelFilterTest.cpp flowmeasure/ConcreteRouteFilterTest.cpp flowmeasure/ConcreteMeasureTest.cpp flowmeasure/ConcreteFlowMeasureTest.cpp FlowSdkHeaderTest.cpp FlowSdkMockHeaderTest.cpp flowmeasure/ConcreteFlowMeasureFiltersTest.cpp)

add_executable(${PROJECT_NAME} ${ALL_FILES})
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

#### DEPENDENCIES
add_dependencies(${PROJECT_NAME}
        flow_plugin_sdk_src
        gtest
        gmock
        )

#### INCLUDES
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/googletest/googlemock/include;"
        )

#### LINKS
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        "${CMAKE_BINARY_DIR}/src/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        gmock
        flow_plugin_sdk_src
        )

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

#### COMPILE OPTIONS
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        /Od;
        -Zm200
        >
        $<$<CONFIG:Release>:
        /O2;
        /Oi;
        /Gy;
        -Zm120
        >
        /std:c++20;
        /sdl;
        /W4;
        /WX;
        ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
        -Wno-unused-parameter # Lots of interfaces don't use everything
        -Wno-missing-field-initializers # Windows has loads of this sadly
        /EHa
        )

#### LINK OPTIONS
target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
        /DEBUG:FASTLINK;
        /INCREMENTAL
        -flto=thin
        >
        $<$<CONFIG:Release>:
        /DEBUG;
        /OPT:REF;
        /OPT:ICF;
        /INCREMENTAL:NO
        >
        /NODEFAULTLIB:LIBCMT;
        /SUBSYSTEM:CONSOLE;
        )
