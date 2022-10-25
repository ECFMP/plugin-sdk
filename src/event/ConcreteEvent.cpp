#include "ConcreteEvent.h"

namespace FlowSdk::Event {

    ConcreteEvent::ConcreteEvent(int id, std::string name, std::chrono::system_clock::time_point start,
                                 std::chrono::system_clock::time_point end,
                                 std::shared_ptr<FlightInformationRegion::FlightInformationRegion> fir)
        : id(id), name(std::move(name)), start(start), end(end), fir(std::move(fir))
    {}

    auto FlowSdk::Event::ConcreteEvent::Id() const -> int
    {
        return id;
    }

    auto FlowSdk::Event::ConcreteEvent::Name() const -> const std::string&
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
}// namespace FlowSdk::Event
