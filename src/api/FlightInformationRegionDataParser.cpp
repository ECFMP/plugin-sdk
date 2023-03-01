#include "FlightInformationRegionDataParser.h"
#include "api/ConcreteApiElementCollection.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flow-sdk/Logger.h"
#include "nlohmann/json.hpp"

namespace FlowSdk::Api {

    FlightInformationRegionDataParser::FlightInformationRegionDataParser(
            std::shared_ptr<InternalFlightInformationRegionCollection> firs, std::shared_ptr<Log::Logger> logger
    )
        : firs(std::move(firs)), logger(std::move(logger))
    {
        assert(this->firs && "firs not set in FlightInformationRegionDataParser");
        assert(this->logger && "logger not set in FlightInformationRegionDataParser");
    }

    void FlightInformationRegionDataParser::OnEvent(const nlohmann::json& data)
    {
        ConcreteApiElementCollection<FlightInformationRegion::FlightInformationRegion> fir2;

        logger->Debug("Updating FIRs");
        if (!DataIsValid(data)) {
            logger->Error("Invalid FIR data from API");
            return;
        }

        for (const auto& fir: data) {
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
    }

    auto FlightInformationRegionDataParser::DataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_array();
    }

    auto FlightInformationRegionDataParser::FirDataIsValid(const nlohmann::json& data) -> bool
    {
        return data.is_object() && data.contains("id") && data.at("id").is_number_integer()
                && data.contains("identifier") && data.at("identifier").is_string() && data.contains("name")
                && data.at("name").is_string();
    }
}// namespace FlowSdk::Api
