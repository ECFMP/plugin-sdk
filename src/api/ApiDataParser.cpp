#include "ApiDataParser.h"
#include "ECFMP/log/Logger.h"
#include "EventDataParserInterface.h"
#include "FlightInformationRegionDataParserInterface.h"
#include "FlowMeasureDataParserInterface.h"

namespace ECFMP::Api {

    ApiDataParser::ApiDataParser(
            std::shared_ptr<EventDataParserInterface> eventParser,
            std::shared_ptr<FlightInformationRegionDataParserInterface> firParser,
            std::shared_ptr<FlowMeasureDataParserInterface> flowMeasureParser, std::shared_ptr<Log::Logger> logger
    )
        : eventParser(std::move(eventParser)), firParser(std::move(firParser)),
          flowMeasureParser(std::move(flowMeasureParser)), logger(std::move(logger))
    {
        assert(this->eventParser != nullptr && "EventDataParser cannot be null");
        assert(this->firParser != nullptr && "FlightInformationRegionDataParser cannot be null");
        assert(this->flowMeasureParser != nullptr && "FlowMeasureDataParser cannot be null");
        assert(this->logger != nullptr && "Logger cannot be null");
    }

    void ApiDataParser::OnEvent(const ApiDataDownloadedEvent& event)
    {
        auto firs = this->firParser->ParseFirs(event.data);

        // No FIRs available, so we've got no chance of being able to parse any events.
        if (!firs) {
            this->logger->Warning("Could not parse FIRs. Events and flow measures will not be parsed.");
            return;
        }

        // No events available, so we've got no chance of being able to parse any flow measures.
        auto events = this->eventParser->ParseEvents(event.data, *firs);
        if (!events) {
            this->logger->Warning("Could not parse events. Flow measures will not be parsed.");
            return;
        }

        auto flowMeasures = this->flowMeasureParser->ParseFlowMeasures(event.data, *events, *firs);
        if (!flowMeasures) {
            this->logger->Warning("Could not parse flow measures.");
            return;
        }
    }
}// namespace ECFMP::Api
