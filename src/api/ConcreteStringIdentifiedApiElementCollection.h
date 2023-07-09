#pragma once
#include "InternalStringIdentifiedApiElementCollection.h"
#include "flow-sdk/ApiElement.h"

namespace ECFMP::Api {
    template<typename T>
    class ConcreteStringIdentifiedApiElementCollection : public InternalStringIdentifiedApiElementCollection<T>
    {
        public:
        void Add(const std::shared_ptr<T>& element) noexcept override;
        auto Count() const noexcept -> size_t override;
        auto Get(int id) const noexcept -> std::shared_ptr<const T> override;
        auto GetByIdentifier(const std::string& identifier) const noexcept -> std::shared_ptr<const T> override;
        auto begin() const -> ApiElementIterator<T> override;
        auto end() const -> ApiElementIterator<T> override;
        auto cbegin() const -> ApiElementIterator<T> override;
        auto cend() const -> ApiElementIterator<T> override;

        private:
        // Elements
        std::map<int, std::shared_ptr<T>> elementsById;
        std::map<std::string, std::shared_ptr<T>> elementsByIdentifier;

        // For locking
        mutable std::recursive_mutex lock;

        // Static assertion to make sure that everything is do-able
        static_assert(std::is_base_of<Api::StringIdentifiableApiElement, T>(), "Element must implement API element");
    };
}// namespace ECFMP::Api
