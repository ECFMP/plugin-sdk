#pragma once
#include "EventListener.h"
#include "EventListeners.h"
#include "FlowMeasure.h"

namespace FlowSdk::FlowMeasure {
    class FlowMeasure;
}// namespace FlowSdk::FlowMeasure

namespace FlowSdk::Plugin {

    // Collection types
    typedef EventListeners<FlowMeasure::FlowMeasure> SingleFlowMeasureListenerCollection;
    typedef EventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure> DoubleFlowMeasureListenerCollection;

    typedef SingleFlowMeasureListenerCollection FlowMeasureNotifiedEventListeners;
    typedef SingleFlowMeasureListenerCollection FlowMeasureActivatedEventListeners;
    typedef SingleFlowMeasureListenerCollection FlowMeasureExpiredEventListeners;
    typedef SingleFlowMeasureListenerCollection FlowMeasureWithdrawnEventListeners;
    typedef DoubleFlowMeasureListenerCollection FlowMeasureReissuedEventListeners;

    // Listener types
    typedef EventListener<FlowSdk::FlowMeasure::FlowMeasure> SingleFlowMeasureListener;
    typedef EventListener<FlowSdk::FlowMeasure::FlowMeasure, FlowSdk::FlowMeasure::FlowMeasure>
            DoubleFlowMeasureListener;

    typedef SingleFlowMeasureListener FlowMeasureNotifiedListener;
    typedef SingleFlowMeasureListener FlowMeasureActivatedListener;
    typedef SingleFlowMeasureListener FlowMeasureExpiredListener;
    typedef SingleFlowMeasureListener FlowMeasureWithdrawnListener;
    typedef DoubleFlowMeasureListener FlowMeasureReissuedListener;
}// namespace FlowSdk::Plugin
