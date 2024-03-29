#pragma once
#include "ECFMP/api/ApiElementCollection.h"
#include <memory>
#include <mutex>

namespace ECFMP::Api {
    template<typename T>
    class InternalApiElementCollection : public ApiElementCollection<T>
    {
        public:
        void Add(const std::shared_ptr<T>& element) noexcept
        {
            const auto uniqueLock = std::unique_lock(this->lock);
            this->elements[static_cast<std::shared_ptr<T>>(element)->Id()] = element;
        }
    };
}// namespace ECFMP::Api
