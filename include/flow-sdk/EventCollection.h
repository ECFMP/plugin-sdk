#pragma once
#include "ApiElementCollection.h"

namespace FlowSdk::Event {
    class Event;

    class EventCollection : Api::ApiElementCollection<Event>
    {
        public:
        /**
         * Gets an Event by its VATCAN events code.
         *
         * Returns nullptr if not found.
         */
        [[nodiscard]] virtual auto GetByVatcanCode(const std::string& code) const noexcept
                -> std::shared_ptr<Event> = 0;
    };
}// namespace FlowSdk::Event
