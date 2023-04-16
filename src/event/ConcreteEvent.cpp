#include "ConcreteEvent.h"

namespace FlowSdk::Event {

    ConcreteEvent::ConcreteEvent(
            int id, std::string name, std::chrono::system_clock::time_point start,
            std::chrono::system_clock::time_point end,
            std::shared_ptr<const FlightInformationRegion::FlightInformationRegion> fir, std::string vatcanCode
    )
        : id(id), name(std::move(name)), start(start), end(end), fir(fir), vatcanCode(std::move(vatcanCode))
    {
        assert(fir && "flight information region not set in event");
    }

    auto FlowSdk::Event::ConcreteEvent::Id() const noexcept -> int
    {
        return id;
    }

    auto FlowSdk::Event::ConcreteEvent::Name() const noexcept -> const std::string&
    {
        return name;
    }

    auto FlowSdk::Event::ConcreteEvent::Start() const noexcept -> const std::chrono::system_clock::time_point&
    {
        return start;
    }

    auto ConcreteEvent::End() const noexcept -> const std::chrono::system_clock::time_point&
    {
        return end;
    }

    auto ConcreteEvent::FlightInformationRegion() const noexcept
            -> const FlightInformationRegion::FlightInformationRegion&
    {
        return *fir;
    }

    auto ConcreteEvent::VatcanCode() const noexcept -> const std::string&
    {
        return vatcanCode;
    }
}// namespace FlowSdk::Event
