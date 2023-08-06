#include "ConcreteEventFilter.h"
#include "ECFMP/event/Event.h"
#include "ECFMP/event/EventParticipant.h"
#include "euroscope/EuroscopeAircraft.h"
#include <algorithm>

namespace ECFMP::FlowMeasure {

    ConcreteEventFilter::ConcreteEventFilter(
            const std::shared_ptr<const ECFMP::Event::Event>& event, EventParticipation participatingIn
    )
        : event(event), participatingIn(participatingIn)
    {
        assert(event && "Event not set in event filter");
    }

    auto ConcreteEventFilter::ApplicableToEvent(const ECFMP::Event::Event& applicableTo) const noexcept -> bool
    {
        return applicableTo == *this->event;
    }

    auto ConcreteEventFilter::Event() const noexcept -> const ECFMP::Event::Event&
    {
        return *this->event;
    }

    auto ConcreteEventFilter::Participation() const noexcept -> EventParticipation
    {
        return participatingIn;
    }

    auto ConcreteEventFilter::IsParticipating() const noexcept -> bool
    {
        return Participation() == EventParticipation::Participating;
    }

    bool ConcreteEventFilter::ApplicableToAircraft(const Euroscope::EuroscopeAircraft& aircraft) const noexcept
    {
        auto cid = aircraft.Cid();

        /*
         * Euroscope doesnt have the concept of a CID, so we can't get this info.
         * This workaround means we can write tests for things, but ultimately we'll
         * need to find a way to get the CID from Euroscope.
         *
         * TODO: Find a way to get the CID in
         */
        if (cid == 0) {
            return true;
        }

        const auto isParticipating =
                std::find_if(
                        event->Participants().cbegin(), event->Participants().cend(),
                        [&aircraft](const std::shared_ptr<const Event::EventParticipant>& participant) {
                            return participant->Cid() == aircraft.Cid();
                        }
                )
                != event->Participants().end();

        return (IsParticipating() && isParticipating) || (!IsParticipating() && !isParticipating);
    }
}// namespace ECFMP::FlowMeasure
