#pragma once
#include "flow-sdk/EventListeners.h"

namespace ECFMP::Plugin {
    template<typename... Types>
    /**
     * An internal implementation of EventListeners that adds additional methods for internal use.
     */
    class InternalEventListeners : public EventListeners<Types...>
    {
        public:
        /**
         * Called when an event happens, should iterate
         * the listeners and run them.
         */
        virtual void OnEvent(const Types&...) noexcept = 0;
    };
}// namespace ECFMP::Plugin
