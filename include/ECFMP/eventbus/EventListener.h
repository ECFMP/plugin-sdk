#pragma once

namespace ECFMP::EventBus {
    template<typename EventType>
    class EventListener
    {
        public:
        virtual ~EventListener() = default;
        virtual void OnEvent(const EventType&) = 0;
    };
}// namespace ECFMP::EventBus
