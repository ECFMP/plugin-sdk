#pragma once
#include <iterator>
#include <memory>

namespace ECFMP::Api {
    template<typename T>
    class ApiElementIterator
    {
        public:
        ApiElementIterator(
                std::recursive_mutex& lockingMutex,
                typename std::unordered_map<int, std::shared_ptr<const T>>::const_iterator current
        );
        ApiElementIterator(const ApiElementIterator& old);
        ~ApiElementIterator();
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;  // or also value_type*
        using reference = T&;// or also value_type&

        auto operator*() const -> const T&;
        auto operator->() const -> const T*;
        auto operator++() -> ApiElementIterator<T>&;
        auto operator++(int) -> ApiElementIterator<T>;
        auto operator==(const ApiElementIterator<T>& compare) const -> bool;
        auto operator!=(const ApiElementIterator<T>& compare) const -> bool;

        private:
        // Implementation
        struct Impl;
        std::shared_ptr<Impl> impl;
    };
}// namespace ECFMP::Api
