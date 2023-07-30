#pragma once

namespace ECFMP::EventBus {

    template<typename EventType>
    class EventFilter
    {
        public:
        virtual ~EventFilter() = default;
        virtual bool ShouldProcess(const EventType& event) = 0;
    };
}// namespace ECFMP::EventBus
