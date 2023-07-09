#include "FlowMeasureMeasureParser.h"
#include "flow-sdk/Logger.h"
#include "flowmeasure/ConcreteMeasure.h"
#include "nlohmann/json.hpp"

namespace ECFMP::Api {

    FlowMeasureMeasureParser::FlowMeasureMeasureParser(std::shared_ptr<Log::Logger> logger) : logger(std::move(logger))
    {
        assert(this->logger != nullptr && "FlowMeasureMeasureParser::FlowMeasureMeasureParser: logger is null");
    }

    std::unique_ptr<FlowMeasure::Measure> FlowMeasureMeasureParser::Parse(const nlohmann::json& data) const
    {
        // Json must be an object
        if (!data.is_object()) {
            logger->Warning("FlowMeasureMeasureParser::Parse: data is not an object");
            return nullptr;
        }

        // Json must have a type field that is a string and a value field containing something
        if (!data.contains("type") || !data["type"].is_string() || !data.contains("value")) {
            logger->Warning("FlowMeasureMeasureParser::Parse: data is missing fields");
            return nullptr;
        }

        // Parse the type
        FlowMeasure::MeasureType type;
        try {
            type = GetType(data["type"].get<std::string>());
        }
        catch (const std::invalid_argument& e) {
            logger->Warning("FlowMeasureMeasureParser::Parse: " + std::string(e.what()));
            return nullptr;
        }

        // If the type is MileInTrail, MinimumDepartureInterval, Per Hour, MaxIndicatedAirspeed,
        // IndicatedAirspeedReduction, then the value must be an integer
        if (type == FlowMeasure::MeasureType::MilesInTrail || type == FlowMeasure::MeasureType::MinimumDepartureInterval
            || type == FlowMeasure::MeasureType::AverageDepartureInterval || type == FlowMeasure::MeasureType::PerHour
            || type == FlowMeasure::MeasureType::MaxIndicatedAirspeed
            || type == FlowMeasure::MeasureType::IndicatedAirspeedReduction) {

            if (!data["value"].is_number_integer())
                return nullptr;

            return std::make_unique<FlowMeasure::ConcreteMeasure>(type, data["value"].get<int>());
        }

        // If the type is MaxMach, MachReduction, then the value must be a double
        if (type == FlowMeasure::MeasureType::MaxMach || type == FlowMeasure::MeasureType::MachReduction) {

            if (!data["value"].is_number_float())
                return nullptr;

            return std::make_unique<FlowMeasure::ConcreteMeasure>(type, data["value"].get<double>());
        }

        // If the type is MandatoryRoute, then the value must be a set of strings
        if (type == FlowMeasure::MeasureType::MandatoryRoute) {

            if (!data["value"].is_array())
                return nullptr;

            std::set<std::string> value;
            for (const auto& element: data["value"]) {
                if (!element.is_string())
                    return nullptr;

                value.insert(element.get<std::string>());
            }

            return std::make_unique<FlowMeasure::ConcreteMeasure>(type, value);
        }

        // Otherwise if it's a prohibit or ground stop, then there is no value
        return std::make_unique<FlowMeasure::ConcreteMeasure>(type);
    }

    auto FlowMeasureMeasureParser::GetType(const std::string& type) -> FlowMeasure::MeasureType
    {
        if (type == "miles_in_trail")
            return FlowMeasure::MeasureType::MilesInTrail;
        if (type == "minimum_departure_interval")
            return FlowMeasure::MeasureType::MinimumDepartureInterval;
        if (type == "average_departure_interval")
            return FlowMeasure::MeasureType::AverageDepartureInterval;
        if (type == "per_hour")
            return FlowMeasure::MeasureType::PerHour;
        if (type == "max_ias")
            return FlowMeasure::MeasureType::MaxIndicatedAirspeed;
        if (type == "max_mach")
            return FlowMeasure::MeasureType::MaxMach;
        if (type == "ias_reduction")
            return FlowMeasure::MeasureType::IndicatedAirspeedReduction;
        if (type == "mach_reduction")
            return FlowMeasure::MeasureType::MachReduction;
        if (type == "prohibit")
            return FlowMeasure::MeasureType::Prohibit;
        if (type == "mandatory_route")
            return FlowMeasure::MeasureType::MandatoryRoute;
        if (type == "ground_stop")
            return FlowMeasure::MeasureType::GroundStop;

        throw std::invalid_argument("Unknown measure type: " + type);
    }
}// namespace ECFMP::Api
