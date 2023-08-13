#include "ConcreteMeasure.h"
#include "ECFMP/flowmeasure/Measure.h"
#include <stdexcept>
#include <string>

namespace ECFMP::FlowMeasure {

    ConcreteMeasure::ConcreteMeasure(MeasureType type) : type(type), valueType(MeasureValueType::None)
    {}

    ConcreteMeasure::ConcreteMeasure(MeasureType type, int value)
        : type(type), valueType(MeasureValueType::Integer), intValue(value)
    {}

    ConcreteMeasure::ConcreteMeasure(MeasureType type, double value)
        : type(type), valueType(MeasureValueType::Double), doubleValue(value)
    {}

    ConcreteMeasure::ConcreteMeasure(MeasureType type, std::set<std::string> value)
        : type(MeasureType::MandatoryRoute), valueType(MeasureValueType::Set), setValue(std::move(value))
    {}

    auto ConcreteMeasure::Type() const noexcept -> MeasureType
    {
        return type;
    }

    auto ConcreteMeasure::IntegerValue() const -> int
    {
        AssertMeasureType(MeasureValueType::Integer);

        return intValue;
    }

    auto ConcreteMeasure::DoubleValue() const -> double
    {
        AssertMeasureType(MeasureValueType::Double);

        return doubleValue;
    }

    auto ConcreteMeasure::SetValue() const -> const std::set<std::string>&
    {
        AssertMeasureType(MeasureValueType::Set);

        return setValue;
    }

    void ConcreteMeasure::AssertMeasureType(MeasureValueType allowedType) const
    {
        if (valueType != allowedType) {
            throw IllegalFlowMeasureValueException();
        }
    }

    auto ConcreteMeasure::MeasureDescription() const noexcept -> std::string
    {
        // No value type associated with these
        if (type == MeasureType::GroundStop || type == MeasureType::Prohibit) {
            return MeasureTypeToString(type);
        }

        return MeasureTypeToString(type) + ": " + MeasureValueToString(type);
    }

    auto ConcreteMeasure::MeasureTypeToString(MeasureType type) -> std::string
    {
        switch (type) {
        case MeasureType::MinimumDepartureInterval:
            return "Minimum Departure Interval";
        case MeasureType::AverageDepartureInterval:
            return "Average Departure Interval";
        case MeasureType::PerHour:
            return "Per Hour";
        case MeasureType::MilesInTrail:
            return "Miles in Trail";
        case MeasureType::MaxIndicatedAirspeed:
            return "Max IAS";
        case MeasureType::IndicatedAirspeedReduction:
            return "IAS Reduction";
        case MeasureType::MaxMach:
            return "Max Mach";
        case MeasureType::MachReduction:
            return "Mach Reduction";
        case MeasureType::MandatoryRoute:
            return "Mandatory Route(s)";
        case MeasureType::Prohibit:
            return "Prohibit";
        case MeasureType::GroundStop:
            return "Ground Stop";
        default:
            return "Unknown";
        }
    }

    auto ConcreteMeasure::MeasureValueToString(MeasureType measureType) const -> std::string
    {
        switch (measureType) {
        case MeasureType::MinimumDepartureInterval:
        case MeasureType::AverageDepartureInterval: {
            const auto extraSeconds = intValue % 60;
            const auto minutes = intValue / 60;

            if (minutes == 0) {
                return std::to_string(extraSeconds) + " seconds";
            }

            if (extraSeconds == 0) {
                return std::to_string(minutes) + " minutes";
            }

            return std::to_string(minutes) + " minutes " + std::to_string(extraSeconds) + " seconds";
        }
        case MeasureType::PerHour:
        case MeasureType::MilesInTrail:
            return std::to_string(intValue);
        case MeasureType::MaxIndicatedAirspeed:
        case MeasureType::IndicatedAirspeedReduction:
            return std::to_string(intValue) + "kts";
        case MeasureType::MaxMach:
        case MeasureType::MachReduction: {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << doubleValue;
            return stream.str();
        }
        case MeasureType::MandatoryRoute: {
            std::string routeString;
            for (const auto& route: setValue) {
                routeString += route + ", ";
            }
            return routeString.substr(0, routeString.size() - 2);
        }
        case MeasureType::Prohibit:
        case MeasureType::GroundStop:
            throw std::logic_error("MeasureType::Prohibit and MeasureType::GroundStop have no value");
        }

        return "Unknown";
    }
}// namespace ECFMP::FlowMeasure
