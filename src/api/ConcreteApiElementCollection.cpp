#include "ConcreteApiElementCollection.h"
#include "flow-sdk/ApiElement.h"
#include "flow-sdk/ApiElementIterator.h"
#include "flow-sdk/FlightInformationRegion.h"

namespace FlowSdk::Api {

    template<typename T>
    auto ConcreteApiElementCollection<T>::Count() const noexcept -> size_t
    {
        const auto uniqueLock = std::unique_lock(lock);
        return elements.size();
    }

    template<typename T>
    void ConcreteApiElementCollection<T>::Add(const std::shared_ptr<T>& element) noexcept
    {
        const auto uniqueLock = std::unique_lock(lock);
        elements[static_cast<std::shared_ptr<ApiElement>>(element)->Id()] = element;
    }

    template<typename T>
    auto ConcreteApiElementCollection<T>::Get(int id) const noexcept -> std::shared_ptr<const T>
    {
        const auto uniqueLock = std::unique_lock(lock);
        return elements.contains(id) ? elements.at(id) : nullptr;
    }

    template<typename T>
    auto ConcreteApiElementCollection<T>::begin() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elements.begin());
    }

    template<typename T>
    auto ConcreteApiElementCollection<T>::end() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elements.end());
    }

    template<typename T>
    auto ConcreteApiElementCollection<T>::cbegin() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elements.cbegin());
    }

    template<typename T>
    auto ConcreteApiElementCollection<T>::cend() const -> ApiElementIterator<T>
    {
        return ApiElementIterator<T>(lock, elements.cend());
    }

    template class FlowSdk::Api::ConcreteApiElementCollection<
            FlowSdk::FlightInformationRegion::FlightInformationRegion>;
}// namespace FlowSdk::Api
