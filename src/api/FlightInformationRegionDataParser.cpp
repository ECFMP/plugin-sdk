#include "FlightInformationRegionDataParser.h"
#include "ECFMP/log/Logger.h"
#include "InternalElementCollectionTypes.h"
#include "eventbus/InternalEventBus.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "nlohmann/json.hpp"
#include "plugin/InternalSdkEvents.h"

namespace ECFMP::Api {

    FlightInformationRegionDataParser::FlightInformationRegionDataParser(
            std::shared_ptr<Log::Logger> logger, std::shared_ptr<EventBus::InternalEventBus> eventBus
    )
        : logger(std::move(logger)), eventBus(std::move(eventBus))
    {
        assert(this->logger && "logger not set in FlightInformationRegionDataParser");
        assert(this->eventBus && "eventBus not set in FlightInformationRegionDataParser");
    }

    auto FlightInformationRegionDataParser::ParseFirs(const nlohmann::json& data)
            -> std::shared_ptr<InternalFlightInformationRegionCollection>
    {
        auto firs = std::make_shared<InternalFlightInformationRegionCollection>();

        logger->Debug("Updating FIRs");
        if (!DataIsValid(data)) {
            logger->Error("Invalid FIR data from API");
            return nullptr;
        }

        for (const auto& fir: data.at("flight_information_regions")) {
            if (!FirDataIsValid(fir)) {
                logger->Error("Invalid FIR in FIR data from API");
                logger->Debug("Failed updating FIR: " + fir.dump());
                continue;
            }

            firs->Add(std::make_shared<FlightInformationRegion::ConcreteFlightInformationRegion>(
                    fir.at("id").get<int>(), fir.at("identifier").get<std::string>(), fir.at("name").get<std::string>()
            ));
        }

        logger->Debug("Finished updating FIRs");
        eventBus->OnEvent<Plugin::FlightInformationRegionsUpdatedEvent>({firs});
        return firs;
    }

    auto FlightInformationRegionDataParser::DataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("flight_information_regions")
                && data.at("flight_information_regions").is_array();
    }

    auto FlightInformationRegionDataParser::FirDataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer()
                && data.contains("identifier") && data.at("identifier").is_string() && data.contains("name")
                && data.at("name").is_string();
    }
}// namespace ECFMP::Api
