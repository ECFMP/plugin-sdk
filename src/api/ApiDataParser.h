#pragma once
#include "ApiDataDownloadedEvent.h"
#include "ECFMP/eventbus/EventListener.h"

namespace ECFMP::Log {
    class Logger;
}// namespace ECFMP::Log

namespace ECFMP::Api {

    class EventDataParserInterface;
    class FlightInformationRegionDataParserInterface;
    class FlowMeasureDataParserInterface;

    class ApiDataParser : public EventBus::EventListener<ApiDataDownloadedEvent>
    {
        public:
        ApiDataParser(
                std::shared_ptr<EventDataParserInterface> eventParser,
                std::shared_ptr<FlightInformationRegionDataParserInterface> firParser,
                std::shared_ptr<FlowMeasureDataParserInterface> flowMeasureParser, std::shared_ptr<Log::Logger> logger
        );
        ~ApiDataParser() = default;
        void OnEvent(const ApiDataDownloadedEvent& event) override;

        private:
        // The parsers
        std::shared_ptr<EventDataParserInterface> eventParser;
        std::shared_ptr<FlightInformationRegionDataParserInterface> firParser;
        std::shared_ptr<FlowMeasureDataParserInterface> flowMeasureParser;

        // Logger
        std::shared_ptr<Log::Logger> logger;
    };

}// namespace ECFMP::Api
