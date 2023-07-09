#pragma once
#include "ECFMP/flowmeasure/Measure.h"

namespace ECFMP::FlowMeasure {

    enum class MeasureValueType
    {
        None,
        Integer,
        Double,
        Set
    };

    class ConcreteMeasure : public Measure
    {
        public:
        explicit ConcreteMeasure(MeasureType type);
        ConcreteMeasure(MeasureType type, int value);
        ConcreteMeasure(MeasureType type, double value);
        ConcreteMeasure(MeasureType type, std::set<std::string> value);

        [[nodiscard]] auto Type() const noexcept -> MeasureType override;
        [[nodiscard]] auto IntegerValue() const -> int override;
        [[nodiscard]] auto DoubleValue() const -> double override;
        [[nodiscard]] auto SetValue() const -> const std::set<std::string>& override;

        private:
        inline void AssertMeasureType(MeasureValueType allowedType) const;

        // Type of the measure
        MeasureType type;

        // The value type of the measure
        MeasureValueType valueType;

        // Integer value
        int intValue = -1;

        // Double value
        double doubleValue = -1.0;

        // Set value
        std::set<std::string> setValue;
    };

}// namespace ECFMP::FlowMeasure
