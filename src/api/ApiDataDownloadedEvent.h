#pragma once
#include "nlohmann/json.hpp"

namespace ECFMP::Api {
    using ApiDataDownloadedEvent = struct ApiDataDownloadedEvent {
        nlohmann::json data;
    };
}// namespace ECFMP::Api
