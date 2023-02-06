#pragma once

namespace FlowSdk::FlightInformationRegion {
    class FlightInformationRegion;
}// namespace FlowSdk::FlightInformationRegion

namespace FlowSdk::FlowMeasure {

    class AirportFilter;
    class EventFilter;
    class LevelFilter;
    class RouteFilter;

    /**
     * Represents a collection of flow measure filters, along with some helper
     * methods for commonly used queries.
     */
    class FlowMeasureFilters
    {
        public:
        virtual ~FlowMeasureFilters() = default;
        /**
         * Returns true if any of the airport filters make the flow measure applicable to
         * the given airport.
         */
        [[nodiscard]] virtual auto ApplicableToAirport(const std::string& airfield) const noexcept -> bool = 0;

        /**
         * Returns true if any of the flight information regions make the flow measure applicable to
         * the given region.
         */
        [[nodiscard]] virtual auto ApplicableToFlightInformationRegion(int firId) const noexcept -> bool = 0;
        [[nodiscard]] virtual auto ApplicableToFlightInformationRegion(const std::string& fir) const noexcept
                -> bool = 0;
        [[nodiscard]] virtual auto
        ApplicableToFlightInformationRegion(const FlightInformationRegion::FlightInformationRegion& fir) const noexcept
                -> bool = 0;

        /**
         * Methods that allow for iteration of the filters.
         */
        virtual void ForEachAirportFilter(const std::function<void(const AirportFilter&)>& callback) const noexcept = 0;
        virtual void ForEachEventFilter(const std::function<void(const EventFilter&)>& callback) const noexcept = 0;
        virtual void ForEachLevelFilter(const std::function<void(const LevelFilter&)>& callback) const noexcept = 0;
        virtual void ForEachRouteFilter(const std::function<void(const RouteFilter&)>& callback) const noexcept = 0;

        /**
         * Returns the first filter where a condition is met.
         */
        [[nodiscard]] virtual auto
        FirstAirportFilter(const std::function<bool(const AirportFilter&)>& callback) const noexcept
                -> std::shared_ptr<AirportFilter> = 0;
        [[nodiscard]] virtual auto
        FirstEventFilter(const std::function<bool(const EventFilter&)>& callback) const noexcept
                -> std::shared_ptr<EventFilter> = 0;
        [[nodiscard]] virtual auto
        FirstLevelFilter(const std::function<bool(const LevelFilter&)>& callback) const noexcept
                -> std::shared_ptr<LevelFilter> = 0;
        [[nodiscard]] virtual auto
        FirstRouteFilter(const std::function<bool(const RouteFilter&)>& callback) const noexcept
                -> std::shared_ptr<RouteFilter> = 0;
    };
}// namespace FlowSdk::FlowMeasure
