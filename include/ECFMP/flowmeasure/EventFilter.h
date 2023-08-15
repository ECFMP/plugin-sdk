#pragma once
#include "ChecksAircraftApplicability.h"

namespace ECFMP::Event {
    class Event;
}// namespace ECFMP::Event

namespace ECFMP::FlowMeasure {

    /**
     * A enum for whether a pilot is participating
     * in an event.
     */
    enum class EventParticipation
    {
        Participating = 0,
        NotParticipating = 1,
    };

    /**
     * A filter that pertains to participation in an event.
     */
    class EventFilter : public ChecksAircraftApplicability
    {
        public:
        virtual ~EventFilter() = default;
        /**
         * Helper method that returns whether or not the filter is applicable to a given event.
         */
        [[nodiscard]] virtual auto ApplicableToEvent(const ECFMP::Event::Event& event) const noexcept -> bool = 0;

        /**
         * Returns the event to which this filter is applicable.
         */
        [[nodiscard]] virtual auto Event() const noexcept -> const ECFMP::Event::Event& = 0;

        /**
         * Returns whether the filter is a "participating in" or "not participating in" filter.
         */
        [[nodiscard]] virtual auto Participation() const noexcept -> EventParticipation = 0;

        /**
         * Returns true if the event participation is "Participating in"
         */
        [[nodiscard]] virtual auto IsParticipating() const noexcept -> bool = 0;

        /**
         * Returns a description of the filter.
         */
        [[nodiscard]] virtual auto FilterDescription() const noexcept -> std::string = 0;
    };
}// namespace ECFMP::FlowMeasure
