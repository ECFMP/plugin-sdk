#pragma once

namespace ECFMP::EventBus {

    template<typename EventType>
    class EventFilter
    {
        public:
        virtual ~EventFilter() = default;
        virtual bool ShouldProcess(const EventType&) = 0;
    };
}// namespace ECFMP::EventBus
