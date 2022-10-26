#include "ConcreteEvent.h"

namespace FlowSdk::Event {

    ConcreteEvent::ConcreteEvent(int id, const char* name, std::chrono::system_clock::time_point start,
                                 std::chrono::system_clock::time_point end,
                                 std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir,
                                 const char* vatcanCode)
        : id(id), name(name), start(start), end(end), fir(std::move(fir)), vatcanCode(vatcanCode)
    {}

    auto FlowSdk::Event::ConcreteEvent::Id() const -> int
    {
        return id;
    }

    auto FlowSdk::Event::ConcreteEvent::Name() const -> const char*
    {
        return name;
    }

    auto FlowSdk::Event::ConcreteEvent::Start() const -> const std::chrono::system_clock::time_point&
    {
        return start;
    }

    auto ConcreteEvent::End() const -> const std::chrono::system_clock::time_point&
    {
        return end;
    }

    auto ConcreteEvent::FlightInformationRegion() const -> const FlightInformationRegion::FlightInformationRegion&
    {
        return *fir;
    }

    auto ConcreteEvent::VatcanCode() const -> const char*
    {
        return vatcanCode;
    }
}// namespace FlowSdk::Event
