#pragma once
#include "flow-sdk/ApiElementCollection.h"

namespace ECFMP::Api {

    /**
     * An internal version of the element collection, which allows
     * for adding items.
     */
    template<typename T>
    class InternalApiElementCollection : public ApiElementCollection<T>
    {
        public:
        /**
         * Adds an element to the collection, replacing it if already there.
         */
        virtual void Add(const std::shared_ptr<T>& element) noexcept = 0;

        /**
         * Returns the number of items in the collection.
         */
        [[nodiscard]] virtual auto Count() const noexcept -> size_t = 0;
    };
}// namespace ECFMP::Api
