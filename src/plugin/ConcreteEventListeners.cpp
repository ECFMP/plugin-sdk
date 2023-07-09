#include "ConcreteEventListeners.h"
#include "flow-sdk/EventListener.h"
#include "flow-sdk/EventListenerFilter.h"
#include "flow-sdk/FlowMeasure.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Plugin {

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
    void ConcreteEventListeners<Types...>::Add(
            std::shared_ptr<EventListener<Types...>> listener, std::shared_ptr<EventListenerFilter<Types...>> filter
    ) noexcept
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
}// namespace ECFMP::Plugin

// Explicit class instantiations

// For single-flow measure events
template class ECFMP::Plugin::ConcreteEventListeners<ECFMP::FlowMeasure::FlowMeasure>;
template class ECFMP::Plugin::EventListenerFilter<ECFMP::FlowMeasure::FlowMeasure>;
template class ECFMP::Plugin::EventListener<ECFMP::FlowMeasure::FlowMeasure>;

// For double-flow measure events
template class ECFMP::Plugin::ConcreteEventListeners<ECFMP::FlowMeasure::FlowMeasure, ECFMP::FlowMeasure::FlowMeasure>;
template class ECFMP::Plugin::EventListenerFilter<ECFMP::FlowMeasure::FlowMeasure, ECFMP::FlowMeasure::FlowMeasure>;
template class ECFMP::Plugin::EventListener<ECFMP::FlowMeasure::FlowMeasure, ECFMP::FlowMeasure::FlowMeasure>;

// For downloading API data
template class ECFMP::Plugin::ConcreteEventListeners<const nlohmann::json&>;
template class ECFMP::Plugin::EventListenerFilter<const nlohmann::json&>;
template class ECFMP::Plugin::EventListener<const nlohmann::json&>;
