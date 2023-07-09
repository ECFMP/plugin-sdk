#pragma once

namespace ECFMP::Plugin {
    /**
     * A filter for events that can be paired with a listener to determine if
     * it should be called.
     */
    template<typename... Types>
    class EventListenerFilter
    {
        public:
        virtual ~EventListenerFilter() = default;
        [[nodiscard]] virtual auto Passes(const Types&...) -> bool = 0;
    };
}// namespace ECFMP::Plugin
