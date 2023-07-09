#include "FlowMeasureDataParser.h"
#include "FlowMeasureFilterParser.h"
#include "FlowMeasureMeasureParser.h"
#include "api/InternalApiElementCollection.h"
#include "date/ParseDateStrings.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasure.h"
#include "flow-sdk/Logger.h"
#include "flowmeasure/ConcreteFlowMeasure.h"
#include "nlohmann/json.hpp"

namespace ECFMP::Api {
    FlowMeasureDataParser::FlowMeasureDataParser(
            std::unique_ptr<FlowMeasureFilterParserInterface> filterParser,
            std::unique_ptr<FlowMeasureMeasureParserInterface> measureParser,
            std::shared_ptr<InternalFlowMeasureCollection> flowMeasures,
            std::shared_ptr<InternalEventCollection> events,
            std::shared_ptr<const InternalFlightInformationRegionCollection> firs, std::shared_ptr<Log::Logger> logger
    )
        : filterParser(std::move(filterParser)), measureParser(std::move(measureParser)),
          flowMeasures(std::move(flowMeasures)), events(std::move(events)), firs(std::move(firs)),
          logger(std::move(logger))
    {
        assert(this->filterParser != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: filterParser is null");
        assert(this->measureParser != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: measureParser is null");
        assert(this->flowMeasures != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: flowMeasures is null");
        assert(this->events != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: events is null");
        assert(this->firs != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: firs is null");
        assert(this->logger != nullptr && "FlowMeasureDataParser::FlowMeasureDataParser: logger is null");
    }

    void FlowMeasureDataParser::OnEvent(const nlohmann::json& data)
    {
        if (!DataIsValid(data)) {
            logger->Error("FlowMeasureDataParser::OnEvent: data is invalid");
            return;
        }

        for (const auto& flowMeasureData: data["flow_measures"]) {
            if (!FlowMeasurePropertiesValid(flowMeasureData)) {
                logger->Error("FlowMeasureDataParser::OnEvent: flow measure properties are invalid");
                continue;
            }

            // Get the notfied firs
            auto notifiedFirs = GetNotifiedFirs(flowMeasureData);
            if (notifiedFirs.empty()) {
                logger->Error("FlowMeasureDataParser::OnEvent: notified firs are empty");
                continue;
            }

            // Get the measure type
            auto measure = measureParser->Parse(flowMeasureData.at("measure"));
            if (measure == nullptr) {
                logger->Error("FlowMeasureDataParser::OnEvent: measure is invalid");
                continue;
            }

            // Get the filters
            auto filters = filterParser->Parse(flowMeasureData.at("filters"), *events);
            if (filters == nullptr) {
                logger->Error("FlowMeasureDataParser::OnEvent: filters are invalid");
                continue;
            }

            // Get the event
            std::shared_ptr<const ECFMP::Event::Event> event;
            if (flowMeasureData.at("event_id").is_number_integer()) {
                event = events->Get(flowMeasureData.at("event_id").get<int>());
                if (event == nullptr) {
                    logger->Error("FlowMeasureDataParser::OnEvent: event is invalid");
                    continue;
                }
            }

            // Get the times
            const auto startTime = Date::TimePointFromDateString(flowMeasureData.at("starttime").get<std::string>());
            const auto endTime = Date::TimePointFromDateString(flowMeasureData.at("endtime").get<std::string>());
            const auto withdrawnAt = flowMeasureData.at("withdrawn_at").is_null()
                    ? (std::chrono::system_clock::time_point::max)()
                    : Date::TimePointFromDateString(flowMeasureData.at("withdrawn_at").get<std::string>());

            // Get the measure status based on the time
            auto measureStatus = this->GetMeasureStatus(startTime, endTime, withdrawnAt);

            // Build the ConcreteFlowMeasure
            auto flowMeasure = std::make_shared<FlowMeasure::ConcreteFlowMeasure>(
                    flowMeasureData.at("id").get<int>(), event, flowMeasureData.at("ident").get<std::string>(),
                    flowMeasureData.at("reason").get<std::string>(), startTime, endTime, withdrawnAt, measureStatus,
                    notifiedFirs, std::move(measure), std::move(filters)
            );
            flowMeasures->Add(flowMeasure);
        }
    }

    auto FlowMeasureDataParser::DataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("flow_measures") && data["flow_measures"].is_array();
    }

    /**
     * Validates that the easy to validate flow measure data is valid.
     */
    auto FlowMeasureDataParser::FlowMeasurePropertiesValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer() && data.contains("ident")
                && data.at("ident").is_string() && data.contains("event_id")
                && (data.at("event_id").is_number_integer() || data.at("event_id").is_null()) && data.contains("reason")
                && data.at("reason").is_string() && data.contains("starttime") && data.at("starttime").is_string()
                && Date::DateStringValid(data.at("starttime").get<std::string>()) && data.contains("endtime")
                && data.at("endtime").is_string() && Date::DateStringValid(data.at("endtime").get<std::string>())
                && data.contains("withdrawn_at")
                && ((data.at("withdrawn_at").is_string()
                     && Date::DateStringValid(data.at("withdrawn_at").get<std::string>()))
                    || data.at("withdrawn_at").is_null())
                && data.contains("measure") && data.contains("filters");
    }

    auto FlowMeasureDataParser::GetNotifiedFirs(const nlohmann::json& data) const
            -> std::vector<std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>>
    {
        if (!data.contains("notified_flight_information_regions")
            || !data.at("notified_flight_information_regions").is_array()) {
            logger->Error("FlowMeasureDataParser::GetNotifiedFirs: data is invalid");
            return {};
        }

        std::vector<std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>> notifiedFirs;
        for (const auto& firId: data.at("notified_flight_information_regions")) {
            if (!firId.is_number_integer()) {
                logger->Error("FlowMeasureDataParser::GetNotifiedFirs: fir is not an integer");
                return {};
            }

            auto fir = firs->Get(firId.get<int>());
            if (fir == nullptr) {
                logger->Error("FlowMeasureDataParser::GetNotifiedFirs: fir is null");
                return {};
            }

            notifiedFirs.push_back(fir);
        }

        return notifiedFirs;
    }

    auto FlowMeasureDataParser::GetMeasureStatus(
            const std::chrono::system_clock::time_point& startTime,
            const std::chrono::system_clock::time_point& endTime,
            const std::chrono::system_clock::time_point& withdrawnTime
    ) const -> FlowMeasure::MeasureStatus
    {
        if (withdrawnTime != (std::chrono::system_clock::time_point::max)()) {
            return FlowMeasure::Withdrawn;
        }

        const auto now = std::chrono::system_clock::now();
        if (now < startTime) {
            return FlowMeasure::Notified;
        }

        if (now > endTime) {
            return FlowMeasure::Expired;
        }

        return FlowMeasure::Active;
    }
}// namespace ECFMP::Api
