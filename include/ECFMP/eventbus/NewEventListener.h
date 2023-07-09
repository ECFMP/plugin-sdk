#pragma once

namespace ECFMP::EventBus {
    template<typename EventType>
    class NewEventListener
    {
        public:
        virtual ~NewEventListener() = default;
        virtual void OnEvent(const EventType&) = 0;
    };
}// namespace ECFMP::EventBus
