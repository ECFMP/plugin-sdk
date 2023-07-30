#pragma once
#include "ECFMP/api/StringIdentifierApiElementCollection.h"
#include <memory>
#include <mutex>

namespace ECFMP::Api {
    template<typename T>
    class InternalStringIdentifierApiElementCollection : public StringIdentifierApiElementCollection<T>
    {
        public:
        void Add(const std::shared_ptr<T>& element) noexcept
        {
            const auto uniqueLock = std::unique_lock(this->lock);
            this->elements[static_cast<std::shared_ptr<T>>(element)->Id()] = element;
        }

        [[nodiscard]] auto GetUnderlyingCollection() const noexcept
                -> const std::unordered_map<int, std::shared_ptr<const T>>&
        {
            return this->elements;
        }
    };
}// namespace ECFMP::Api
