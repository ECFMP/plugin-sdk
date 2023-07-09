#pragma once

namespace FlowSdk::EventBus {

    template<typename EventType>
    class NewEventFilter
    {
        public:
        virtual ~NewEventFilter() = default;
        virtual bool ShouldProcess(const EventType&) = 0;
    };
}// namespace FlowSdk::EventBus
