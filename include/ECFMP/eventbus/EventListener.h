#pragma once

namespace ECFMP::EventBus {
    template<typename EventType>
    class EventListener
    {
        public:
        virtual ~EventListener() = default;
        virtual void OnEvent(const EventType& event) = 0;
    };
}// namespace ECFMP::EventBus
