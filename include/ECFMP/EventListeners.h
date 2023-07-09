#pragma once
#include "ECFMP/flowmeasure/FlowMeasure.h"

namespace ECFMP::Plugin {
    template<typename... Types>
    class EventListener;
    template<typename... Types>
    class EventListenerFilter;

    /**
     * A collection of listeners for various events from the SDK.
     */
    template<typename... Types>
    class EventListeners
    {
        public:
        virtual ~EventListeners() = default;

        /**
         * Adds a listener. Duplicates will be ignored.
         */
        virtual void Add(std::shared_ptr<EventListener<Types...>> listener) noexcept = 0;

        /**
         * Adds a listener, with a filter. Duplicates will be ignored.
         */
        virtual void
        Add(std::shared_ptr<EventListener<Types...>> listener,
            std::shared_ptr<EventListenerFilter<Types...>> filter) noexcept = 0;

        /**
         * Removes a listener
         */
        virtual void Remove(std::shared_ptr<EventListener<Types...>> listener) noexcept = 0;
    };
}// namespace ECFMP::Plugin
