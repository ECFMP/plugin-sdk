#pragma once
#include "flow-sdk/EventListener.h"
#include "nlohmann/json_fwd.hpp"

namespace ECFMP::Api {
    typedef Plugin::EventListener<const nlohmann::json&> ApiDataListener;
}// namespace ECFMP::Api
