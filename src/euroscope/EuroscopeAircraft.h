#pragma once

namespace ECFMP::Euroscope {

    /**
     * A class that represents an aircraft in Euroscope, encapsulating the
     * flightplan and radar target.
     */
    class EuroscopeAircraft
    {
        public:
        virtual ~EuroscopeAircraft() = default;

        /**
         * Returns the aircraft's vatsim CID.
         */
        [[nodiscard]] virtual auto Cid() const -> int = 0;

        /**
         * Returns the aircraft's current altitude at standard pressure.
         */
        [[nodiscard]] virtual auto CurrentAltitudeStandardPressure() const -> int = 0;

        /**
         * Returns the aircraft's flight planned cruise altitude.
         */
        [[nodiscard]] virtual auto CruiseAltitude() const -> int = 0;

        /**
         * Returns the aircrafts departure airport.
         */
        [[nodiscard]] virtual auto DepartureAirport() const -> std::string = 0;

        /**
         * Returns the aircrafts destination airport.
         */
        [[nodiscard]] virtual auto DestinationAirport() const -> std::string = 0;

        /**
         * Returns the aircraft's range to destination
         */
        [[nodiscard]] virtual auto RangeToDestination() const -> double = 0;

        /**
         * Returns the aircraft's route string.
         */
        [[nodiscard]] virtual auto RouteString() const -> std::string = 0;

        /**
         * Returns the aircraft's ground speed.
         */
        [[nodiscard]] virtual auto GroundSpeed() const -> int = 0;
    };
}// namespace ECFMP::Euroscope
