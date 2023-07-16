#pragma once
#include "ApiElementIterator.h"

namespace ECFMP::Api {

    /**
     * A collection of elements from the API.
     */
    template<typename T>
    class ApiElementCollection
    {
        public:
        virtual ~ApiElementCollection() = default;

        /**
         * Gets the first item matching some predicate.
         */
        [[nodiscard]] auto First(const std::function<bool(const T&)>& predicate) const noexcept
                -> std::shared_ptr<const T>
        {
            const auto lockGuard = std::lock_guard(lock);
            auto it = std::find_if(elements.cbegin(), elements.cend(), [&predicate](const auto& item) {
                return predicate(*item.second);
            });
            return it == elements.end() ? nullptr : it->second;
        };

        /**
         * Gets an item from the collection by its API id.
         */
        [[nodiscard]] auto Get(int id) const noexcept -> std::shared_ptr<const T>
        {
            const auto lockGuard = std::lock_guard(lock);
            auto it = elements.find(id);
            return it == elements.end() ? nullptr : it->second;
        };

        /**
         * Gets the number of elements in the collection.
         */
        [[nodiscard]] auto Count() const noexcept -> size_t
        {
            const auto lockGuard = std::lock_guard(lock);
            return elements.size();
        };

        /**
         * Checks if the collection contains an element with the given id.
         */
        [[nodiscard]] auto Contains(int id) const noexcept -> bool
        {
            const auto lockGuard = std::lock_guard(lock);
            return elements.contains(id);
        };

        /**
         * Checks if the collection contains an element that satisfies the given predicate.
         */
        [[nodiscard]] auto Contains(const std::function<bool(const T&)>& predicate) const noexcept -> bool
        {
            const auto lockGuard = std::lock_guard(lock);
            return std::any_of(elements.cbegin(), elements.cend(), [&predicate](const auto& item) {
                return predicate(*item.second);
            });
        }

        /**
         * Iterators.
         */
        auto begin() const -> ApiElementIterator<T>
        {
            return ApiElementIterator<T>(lock, elements.begin());
        }

        auto end() const -> ApiElementIterator<T>
        {
            return ApiElementIterator<T>(lock, elements.end());
        }

        auto cbegin() const -> ApiElementIterator<T>
        {
            return ApiElementIterator<T>(lock, elements.cbegin());
        }

        auto cend() const -> ApiElementIterator<T>
        {
            return ApiElementIterator<T>(lock, elements.cend());
        }

        protected:
        // The elements
        std::unordered_map<int, std::shared_ptr<const T>> elements;

        // A mutex for locking
        mutable std::recursive_mutex lock;
    };
}// namespace ECFMP::Api
