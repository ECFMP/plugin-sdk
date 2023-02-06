#pragma once

namespace FlowSdk {
    namespace Event {
        class Event;
    }// namespace Event
    namespace FlightInformationRegion {
        class FlightInformationRegion;
    }// namespace FlightInformationRegion
}// namespace FlowSdk

namespace FlowSdk::FlowMeasure {
    class FlowMeasureFilters;
    class Measure;

    /**
     * Represents the status of the flow measure
     */
    enum MeasureStatus
    {
        Notified = 0,
        Active = 1,
        Withdrawn = 2,
        Expired = 3,
    };

    class FlowMeasureNotWithdrawnException : public std::logic_error
    {
        public:
        explicit FlowMeasureNotWithdrawnException() : std::logic_error("Flow measure not withdrawn")
        {}
    };

    /**
     * Represents an individual flow measure in the API.
     */
    class FlowMeasure
    {
        public:
        virtual ~FlowMeasure() = default;
        /**
         * The id of the measure.
         */
        [[nodiscard]] virtual auto Id() const noexcept -> int = 0;

        /**
         * Returns the event associated with this flow measure, or nullptr if
         * not found.
         */
        [[nodiscard]] virtual auto Event() const noexcept -> std::shared_ptr<const Event::Event> = 0;

        /**
         * The identifier of the measure, e.g. EGTT05A, EHAA15A-B.
         */
        [[nodiscard]] virtual auto Identifier() const noexcept -> const std::string& = 0;

        /**
         * The reason why the measure has been put in place.
         */
        [[nodiscard]] virtual auto Reason() const noexcept -> const std::string& = 0;

        /**
         * The Unix Timestamp for the start time of the flow measure.
         */
        [[nodiscard]] virtual auto StartTime() const noexcept -> const std::chrono::system_clock::time_point& = 0;

        /**
         * The Unix Timestamp for the end time of the flow measure.
         */
        [[nodiscard]] virtual auto EndTime() const noexcept -> const std::chrono::system_clock::time_point& = 0;

        /**
         * The Unix Timestamp for the time the flow measure was withdrawn.
         *
         * Throws a FlowMeasureNotWithdrawnException if not withdrawn.
         */
        [[nodiscard]] virtual auto WithdrawnAt() const -> const std::chrono::system_clock::time_point& = 0;

        /**
         * The current status of the flow measure.
         */
        [[nodiscard]] virtual auto Status() const noexcept -> MeasureStatus = 0;

        /**
         * Checks if the flow measure has the given status.
         */
        [[nodiscard]] virtual auto HasStatus(MeasureStatus status) const noexcept -> bool = 0;

        /**
         * The flight information region(s) associated with the measure.
         */
        [[nodiscard]] virtual auto NotifiedFlightInformationRegions() const noexcept
                -> const std::set<std::shared_ptr<FlightInformationRegion::FlightInformationRegion>> = 0;

        /**
         * The measure itself, type and value.
         */
        [[nodiscard]] virtual auto Measure() const noexcept -> const Measure& = 0;

        /**
         * Returns a collection of filters that apply to the flow measure.
         */
        [[nodiscard]] virtual auto Filters() const noexcept -> const FlowMeasureFilters& = 0;
    };
}// namespace FlowSdk::FlowMeasure
