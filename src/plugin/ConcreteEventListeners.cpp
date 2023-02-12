#include "ConcreteEventListeners.h"
#include "flow-sdk/EventListener.h"
#include "flow-sdk/EventListenerFilter.h"
#include "flow-sdk/FlowMeasure.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk::Plugin {

    /**
     * A filter that always passes.
     */
    template<typename... Types>
    class AlwaysPassFilter : public EventListenerFilter<Types...>
    {
        public:
        auto Passes(const Types&...) -> bool override
        {
            return true;
        }
    };

    template<typename... Types>
    void ConcreteEventListeners<Types...>::Add(std::shared_ptr<EventListener<Types...>> listener) noexcept
    {
        Add(listener, std::make_shared<AlwaysPassFilter<Types...>>());
    }

    template<typename... Types>
    void ConcreteEventListeners<Types...>::Add(std::shared_ptr<EventListener<Types...>> listener,
                                               std::shared_ptr<EventListenerFilter<Types...>> filter) noexcept
    {
        if (listeners.contains(listener)) {
            return;
        }

        listeners[listener] = filter;
    }

    template<typename... Types>
    void ConcreteEventListeners<Types...>::Remove(std::shared_ptr<EventListener<Types...>> listener) noexcept
    {
        listeners.erase(listener);
    }

    template<typename... Types>
    void ConcreteEventListeners<Types...>::OnEvent(const Types&... types) noexcept
    {
        for (const auto& listenerFilterPair: listeners) {
            if (!listenerFilterPair.second->Passes(types...)) {
                continue;
            }

            listenerFilterPair.first->OnEvent(types...);
        }
    }
}// namespace FlowSdk::Plugin

// Explicit class instantiations

// For single-flow measure events
template class FlowSdk::Plugin::ConcreteEventListeners<FlowSdk::FlowMeasure::FlowMeasure>;
template class FlowSdk::Plugin::EventListenerFilter<FlowSdk::FlowMeasure::FlowMeasure>;
template class FlowSdk::Plugin::EventListener<FlowSdk::FlowMeasure::FlowMeasure>;

// For double-flow measure events
template class FlowSdk::Plugin::ConcreteEventListeners<FlowSdk::FlowMeasure::FlowMeasure,
                                                       FlowSdk::FlowMeasure::FlowMeasure>;
template class FlowSdk::Plugin::EventListenerFilter<FlowSdk::FlowMeasure::FlowMeasure,
                                                    FlowSdk::FlowMeasure::FlowMeasure>;
template class FlowSdk::Plugin::EventListener<FlowSdk::FlowMeasure::FlowMeasure, FlowSdk::FlowMeasure::FlowMeasure>;

// For downloading API data
template class FlowSdk::Plugin::ConcreteEventListeners<const nlohmann::json&>;
template class FlowSdk::Plugin::EventListenerFilter<const nlohmann::json&>;
template class FlowSdk::Plugin::EventListener<const nlohmann::json&>;
