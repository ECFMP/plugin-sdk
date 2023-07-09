#pragma once
#include "ApiElementIterator.h"

namespace ECFMP::Plugin {

    /**
     * Represents a class that can be iterated.
     */
    template<typename T>
    class Iterable
    {
        public:
        [[nodiscard]] virtual auto begin() const -> Api::ApiElementIterator<T> = 0;
        [[nodiscard]] virtual auto end() const -> Api::ApiElementIterator<T> = 0;
        [[nodiscard]] virtual auto cbegin() const -> Api::ApiElementIterator<T> = 0;
        [[nodiscard]] virtual auto cend() const -> Api::ApiElementIterator<T> = 0;
    };
}// namespace ECFMP::Plugin
