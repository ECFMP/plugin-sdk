cmake_minimum_required(VERSION 3.23)
project(flow_plugin_sdk_src)
SET(PROJECT_NAME flow_plugin_sdk_src)

set(CMAKE_CXX_STANDARD 20)

set(src_api api/ApiDataDownloader.cpp api/ApiDataDownloader.h api/ApiDataScheduler.cpp api/ApiDataScheduler.h api/FlightInformationRegionDataParser.cpp api/FlightInformationRegionDataParser.h api/ApiDataListenerTypes.h api/InternalApiElementCollection.h api/InternalStringIdentifiedApiElementCollection.h api/ConcreteApiElementCollection.cpp api/ConcreteApiElementCollection.h api/ApiElementIterator.cpp api/ConcreteStringIdentifiedApiElementCollection.cpp api/ConcreteStringIdentifiedApiElementCollection.h api/InternalElementCollectionTypes.h api/EventDataParser.cpp api/EventDataParser.h api/FlowMeasureDataParser.cpp api/FlowMeasureDataParser.h api/FlowMeasureMeasureParserInterface.h api/FlowMeasureFilterParser.cpp api/FlowMeasureFilterParser.h)

set(src_date date/ParseDateStrings.cpp date/ParseDateStrings.h)

set(src_flight_information_regions
        flightinformationregion/ConcreteFlightInformationRegion.h flightinformationregion/ConcreteFlightInformationRegion.cpp)

set(src_events
        event/ConcreteEvent.cpp event/ConcreteEvent.h
        event/ConcreteEventParticipant.cpp "event/ConcreteEventParticipant.h"
        event/Event.cpp ../include/flow-sdk/EventStream.h ../include/flow-sdk/NewEventListener.h ../include/flow-sdk/NewEventFilter.h)

set(src_eventbus
        ../include/flow-sdk/EventStream.h
        ../include/flow-sdk/NewEventListener.h
        ../include/flow-sdk/NewEventFilter.h eventbus/InternalEventStream.h ../include/flow-sdk/EventBus.h eventbus/InternalEventBus.h)

set(src_flowmeasures
        flowmeasure/ConcreteFlowMeasure.cpp flowmeasure/ConcreteFlowMeasure.h flowmeasure/ConcreteAirportFilter.cpp flowmeasure/ConcreteAirportFilter.h flowmeasure/ConcreteEventFilter.cpp flowmeasure/ConcreteEventFilter.h flowmeasure/ConcreteLevelRangeFilter.cpp flowmeasure/ConcreteLevelRangeFilter.h flowmeasure/ConcreteRouteFilter.cpp flowmeasure/ConcreteRouteFilter.h flowmeasure/ConcreteMeasure.cpp flowmeasure/ConcreteMeasure.h flowmeasure/ConcreteMeasureFactory.cpp flowmeasure/ConcreteMeasureFactory.h flowmeasure/ConcreteFlowMeasureFilters.cpp flowmeasure/ConcreteFlowMeasureFilters.h api/FlowMeasureFilterParserInterface.h ../include/flow-sdk/MultipleLevelFilter.h flowmeasure/ConcreteMultipleLevelFilter.cpp flowmeasure/ConcreteMultipleLevelFilter.h ../include/mock/MultipleLevelFilterMock.h ../include/flow-sdk/RangeToDestinationFilter.h flowmeasure/ConcreteRangeToDestinationFilter.cpp flowmeasure/ConcreteRangeToDestinationFilter.h)

set(src_log log/LogDecorator.cpp log/LogDecorator.h log/LogDecorator.h log/NullLogger.cpp log/NullLogger.h)

set(src_pch
        pch.h pch.cpp)

set(src_plugin
        plugin/ConcreteSdk.h
        plugin/ConcreteSdk.cpp
        plugin/InternalEventListeners.h
        plugin/ConcreteEventListeners.cpp plugin/ConcreteEventListeners.h plugin/ConcreteSdkEventListeners.cpp plugin/ConcreteSdkEventListeners.h plugin/SdkFactory.cpp)

set(ALL_FILES
        ${src_api}
        ${src_date}
        ${src_events}
        ${src_eventbus}
        ${src_flowmeasures}
        ${src_flight_information_regions}
        ${src_log}
        ${src_pch}
        ${src_plugin} api/FlowMeasureMeasureParser.cpp api/FlowMeasureMeasureParser.h)

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32" JSON_MultipleHeaders "ON")
target_precompile_headers(${PROJECT_NAME} PRIVATE "pch.h")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR};"
        "${CMAKE_CURRENT_SOURCE_DIR}/../include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../third_party/nlohmann;"
        )

target_compile_options(${PROJECT_NAME} PRIVATE
        /EHa
        )
