#pragma once
#include "InternalEventListeners.h"

namespace FlowSdk::Plugin {

    /**
     * Implements the event listeners
     */
    template<typename... Types>
    class ConcreteEventListeners : public InternalEventListeners<Types...>
    {
        public:
        void Add(std::shared_ptr<EventListener<Types...>> listener) noexcept override;
        void
        Add(std::shared_ptr<EventListener<Types...>> listener,
            std::shared_ptr<EventListenerFilter<Types...>> filter) noexcept override;
        void Remove(std::shared_ptr<EventListener<Types...>> listener) noexcept override;
        void OnEvent(const Types&... types) noexcept override;

        private:
        // All the listeners, with their filters
        std::map<std::shared_ptr<EventListener<Types...>>, std::shared_ptr<EventListenerFilter<Types...>>> listeners;
    };
}// namespace FlowSdk::Plugin
