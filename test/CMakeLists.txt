cmake_minimum_required(VERSION 3.23)
project(ecfmp_sdk_tests)
SET(PROJECT_NAME ecfmp_sdk_tests)

set(CMAKE_CXX_STANDARD 20)
enable_testing()

#### GOOGLE TEST SETUP
include(GoogleTest)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/../third_party/googletest" "third_party/googletest")

#### FILES IN THIS PROJECT AND EXECUTABLE
set(test__api
        api/ApiDataDownloaderTest.cpp
        api/FlightInformationRegionDataParserTest.cpp
        api/ApiElementIteratorTest.cpp api/InternalApiElementCollectionTest.cpp api/FlowMeasureMeasureParserTest.cpp api/FlowMeasureFilterParserTest.cpp
        api/ApiDataParserTest.cpp api/InternalStringIdentifierApiElementCollectionTest.cpp api/ApiDataSchedulerTest.cpp)

set(test__date
        
        date/ParseDateStringsTest.cpp)

set(test__event
        event/ConcreteEventTest.cpp
        event/ConcreteEventParticipantTest.cpp
        api/EventDataParserTest.cpp)

set(test__eventbus
        eventbus/EventStreamTest.cpp
        eventbus/InternalEventBusTest.cpp ../src/eventbus/InternalEventBusFactory.h eventbus/EventDispatcherFactoryTest.cpp)

set(test__flightinformationregion
        flightinformationregion/ConcreteFlightInformationRegionTest.cpp
)

set(test__flowmeasure
        flowmeasure/ConcreteAirportFilterTest.cpp
        flowmeasure/ConcreteEventFilterTest.cpp
        flowmeasure/ConcreteLevelRangeFilterTest.cpp
        flowmeasure/ConcreteRouteFilterTest.cpp
        flowmeasure/ConcreteMeasureTest.cpp
        flowmeasure/ConcreteFlowMeasureTest.cpp
        flowmeasure/ConcreteFlowMeasureFiltersTest.cpp
        api/FlowMeasureDataParserTest.cpp flowmeasure/ConcreteRangeToDestinationFilterTest.cpp flowmeasure/CanonicalFlowMeasureInfoTest.cpp flowmeasure/FlowMeasureStatusUpdatesTest.cpp)

set(test__log
        log/LogDecoratorTest.cpp
)

set(test__mock
        mock/MockEuroscopeAircraft.h
        mock/MockLogger.h
        mock/MockHttpClient.h
        mock/MockEuroscopeAircraftFactory.h ../include/mock/SdkMock.h)

set(test__pch
        pch/pch.cpp
        pch/pch.h
)

set(test__plugin
        plugin/SdkFactoryTest.cpp
        plugin/InternalSdkTest.cpp)

set(test__thread
        thread/ThreadPoolTest.cpp)

set(test__other
        main.cpp
        pch/pch.cpp pch/pch.h
        SdkHeaderTest.cpp
        MockHeaderTest.cpp
        flowmeasure/ConcreteMultipleLevelFilterTest.cpp)

add_executable(${PROJECT_NAME}
        ${test__api}
        ${test__date}
        ${test__event}
        ${test__eventbus}
        ${test__flightinformationregion}
        ${test__flowmeasure}
        ${test__log}
        ${test__mock}
        ${test__plugin}
        ${test__thread}
        ${test__other}
)
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch/pch.h")

#### DEPENDENCIES
add_dependencies(${PROJECT_NAME}
        ecfmp_sdk
        gtest
        gmock
)

#### INCLUDES
target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/.;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/googletest/googlemock/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nlohmann;"
)

# Treat Euroscope as a system include directory to suppress warning, because they have lots
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/euroscope;"
)

#### LINKS
target_link_directories(
        ${PROJECT_NAME} PUBLIC
        "${CMAKE_BINARY_DIR}/src/"
        "${CMAKE_CURRENT_SOURCE_DIR}/../lib"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        gmock
        ecfmp_sdk
        "EuroScopePlugInDll;"
)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS " -m32" LINK_FLAGS "-m32" JSON_MultipleHeaders "ON ")

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
        ${DEFAULT_CXX_EXCEPTION_HANDLING};
        -Wno-unused-parameter; # Lots of interfaces don't use everything
        -Wno-missing-field-initializers; # Windows has loads of this sadly
        /EHa;
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

# Post-build copy the EuroScope binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/../lib/EuroScopePlugInDll.dll" "${PROJECT_BINARY_DIR}/EuroScopePlugInDll.dll"
        COMMENT "Copied EuroScope shared library to ${PROJECT_BINARY_DIR}/EuroScopePlugInDll.dll"
)
