#include "ConcreteMeasure.h"

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
}// namespace ECFMP::FlowMeasure
