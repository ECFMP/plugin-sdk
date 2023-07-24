#pragma once

namespace ECFMP::EventBus {
    /**
 * The dispatch mode for events
 */
    enum EventDispatchMode
    {
        // Event should be dispatched synchronously
        Sync = 0,

        // Event should be dispatched asynchronously
        Async = 1,

        // Event should be retained and later dispatched when the ES thread comes around
        Euroscope = 2
    };

    using SubscriptionFlags = struct SubscriptionFlags {
        // How to dispatch events to the subscription
        EventDispatchMode dispatchMode;

        // Whether we should only run this subscription once
        bool once;
    };

}// namespace ECFMP::EventBus
