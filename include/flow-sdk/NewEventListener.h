#pragma once

namespace FlowSdk::EventBus {
    template<typename EventType>
    class NewEventListener
    {
        public:
        virtual ~NewEventListener() = default;
        virtual void OnEvent(const EventType&) = 0;
    };
}// namespace FlowSdk::EventBus
