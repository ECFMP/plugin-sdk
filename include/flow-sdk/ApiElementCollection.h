#pragma once
#include "flow-sdk/ApiElementIterator.h"
#include "flow-sdk/Iterable.h"

namespace FlowSdk::Api {

    /**
     * A collection of elements from the API.
     */
    template<typename T>
    class ApiElementCollection : Plugin::Iterable<T>
    {
        public:
        virtual ~ApiElementCollection() = default;

        /**
         * Gets an item from the collection by its API id.
         */
        [[nodiscard]] virtual auto Get(int id) const noexcept -> std::shared_ptr<const T> = 0;

        /**
         * Iterators.
         */
        [[nodiscard]] virtual auto begin() const -> ApiElementIterator<T> override = 0;
        [[nodiscard]] virtual auto end() const -> ApiElementIterator<T> override = 0;
        [[nodiscard]] virtual auto cbegin() const -> ApiElementIterator<T> override = 0;
        [[nodiscard]] virtual auto cend() const -> ApiElementIterator<T> override = 0;
    };

    template<typename T>
    class StringIdentifiedApiElementCollection : public ApiElementCollection<T>
    {
        public:
        /**
         * Gets an item from the collection by its identifier, e.g. for London FIR, "EGTT"
         */
        [[nodiscard]] virtual auto GetByIdentifier(const std::string& identifier) const noexcept ->
                std::shared_ptr<const T> = 0;
    };
}// namespace FlowSdk::Api
