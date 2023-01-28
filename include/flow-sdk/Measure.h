#pragma once

namespace FlowSdk::FlowMeasure {

    /**
     * The different type of flow measures
     */
    enum class MeasureType : int
    {
        MinimumDepartureInterval = 0,
        AverageDepartureInterval = 1,
        PerHour = 2,
        MilesInTrail = 3,
        MaxIndicatedAirspeed = 4,
        MaxMach = 5,
        IndicatedAirspeedReduction = 6,
        MachReduction = 7,
        Prohibit = 8,
        GroundStop = 9,
        MandatoryRoute = 10
    };

    /**
     * An exception that is thrown when trying to retrieve a non-existent value from a flow measure.
     */
    class IllegalFlowMeasureValueException : public std::logic_error
    {
        public:
        IllegalFlowMeasureValueException()
            : std::logic_error("Illegal flow measure value - flow measure is not of "
                               "appropriate type"){};
    };

    /**
     * Represents the measure itself, which may be of any number of types.
     */
    class Measure
    {
        public:
        virtual ~Measure() = default;

        /**
         * The type of measure.
         */
        [[nodiscard]] virtual auto Type() const noexcept -> MeasureType = 0;

        /**
         * Returns the value if the measure is of type (unit):
         *
         * Minimum Departure Interval - (Seconds)
         * Average Departure Interval - (Seconds)
         * Per Hour - (Flights Per Hour)
         * Miles In Trail - (Miles In Trail)
         * Max IAS - (Knots)
         * IAS Reduction - (Knots)
         *
         * Throws an IllegalFlowMeasureValueException exception otherwise.
         */
        [[nodiscard]] virtual auto IntegerValue() const -> int = 0;

        /**
         * Returns the value if the measure is of type (unit):
         *
         * Max Mach (Fraction of Mach 1)
         * Mach Reduction (Fraction of Mach 1)
         *
         * Throws an IllegalFlowMeasureValueException exception otherwise.
         */
        [[nodiscard]] virtual auto DoubleValue() const -> double = 0;

        /**
         * Returns the value if the measure is of type Mandatory Route
         * Throws an IllegalFlowMeasureValueException exception otherwise.
         */
        [[nodiscard]] virtual auto SetValue() const -> const std::set<std::string>& = 0;
    };
}// namespace FlowSdk::FlowMeasure
