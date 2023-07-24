#pragma once

#include "eventbus/InternalEventBus.h"

namespace ECFMP::EventBus {
    [[nodiscard]] auto MakeEventBus() -> std::shared_ptr<InternalEventBus>;
}// namespace ECFMP::EventBus
