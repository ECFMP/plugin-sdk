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

    auto FlowSdk::Event::ConcreteEvent::Start() const -> unsigned int
    {
        return std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
    }

    auto ConcreteEvent::End() const -> unsigned int
    {
        return std::chrono::duration_cast<std::chrono::seconds>(end.time_since_epoch()).count();
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
