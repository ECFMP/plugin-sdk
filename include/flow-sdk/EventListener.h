#pragma once

namespace FlowSdk::Plugin {
    /**
     * A listener that listens for a particular type of event from the SDK.
     */
    template<typename... Types>
    class EventListener
    {
        public:
        virtual ~EventListener() = default;
        virtual void OnEvent(const Types&...) = 0;
    };
}// namespace FlowSdk::Plugin
