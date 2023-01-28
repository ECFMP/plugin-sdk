#include "ConcreteEvent.h"

namespace FlowSdk::Event {

    ConcreteEvent::ConcreteEvent(int id, const char* name, std::chrono::system_clock::time_point start,
                                 std::chrono::system_clock::time_point end,
                                 std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir,
                                 const char* vatcanCode)
        : id(id), name(name), start(start), end(end), fir(fir), vatcanCode(vatcanCode)
    {
        assert(fir && "flight information region not set in event");
    }

    auto FlowSdk::Event::ConcreteEvent::Id() const noexcept -> int
    {
        return id;
    }

    auto FlowSdk::Event::ConcreteEvent::Name() const noexcept -> const char*
    {
        return name;
    }

    auto FlowSdk::Event::ConcreteEvent::Start() const noexcept -> unsigned int
    {
        return std::chrono::duration_cast<std::chrono::seconds>(start.time_since_epoch()).count();
    }

    auto ConcreteEvent::End() const noexcept -> unsigned int
    {
        return std::chrono::duration_cast<std::chrono::seconds>(end.time_since_epoch()).count();
    }

    auto ConcreteEvent::FlightInformationRegion() const noexcept
            -> const FlightInformationRegion::FlightInformationRegion&
    {
        return *fir;
    }

    auto ConcreteEvent::VatcanCode() const noexcept -> const char*
    {
        return vatcanCode;
    }
}// namespace FlowSdk::Event
