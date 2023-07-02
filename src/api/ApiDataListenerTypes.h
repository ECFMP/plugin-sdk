#pragma once
#include "flow-sdk/EventListener.h"
#include "nlohmann/json_fwd.hpp"

namespace FlowSdk::Api {
    typedef Plugin::EventListener<const nlohmann::json&> ApiDataListener;
}// namespace FlowSdk::Api
