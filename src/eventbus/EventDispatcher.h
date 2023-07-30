#pragma once

namespace ECFMP::EventBus {
    /**
     * A class responsible for dispatching events to their listeners.
     */
    template<typename EventType>
    class EventDispatcher
    {
        public:
        virtual ~EventDispatcher() = default;
        virtual void Dispatch(const EventType& event) = 0;
    };
}// namespace ECFMP::EventBus
