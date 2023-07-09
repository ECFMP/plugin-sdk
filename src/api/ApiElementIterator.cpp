#include "ECFMP/ApiElementIterator.h"
#include "ECFMP/Event.h"
#include "ECFMP/FlightInformationRegion.h"
#include "ECFMP/FlowMeasure.h"

namespace ECFMP::Api {

    template<typename T>
    struct ApiElementIterator<T>::Impl {
        // A unique lock for this
        std::unique_lock<std::recursive_mutex> lock;

        // The current iterator position
        typename std::map<int, std::shared_ptr<T>>::const_iterator current;
    };

    template<typename T>
    ApiElementIterator<T>::ApiElementIterator(
            std::recursive_mutex& lockingMutex, typename std::map<int, std::shared_ptr<T>>::const_iterator current
    )
    {
        impl = std::make_shared<ApiElementIterator<T>::Impl>();
        impl->lock = std::unique_lock(lockingMutex);
        impl->current = std::move(current);
    }

    template<typename T>
    ApiElementIterator<T>::ApiElementIterator(const ApiElementIterator& old)
    {
        impl = std::make_shared<ApiElementIterator<T>::Impl>();
        impl->lock = std::unique_lock(*old.impl->lock.mutex());
        impl->current = old.impl->current;
    }

    template<typename T>
    ApiElementIterator<T>::~ApiElementIterator() = default;

    template<typename T>
    auto ApiElementIterator<T>::operator*() const -> T&
    {
        return *impl->current->second;
    }

    template<typename T>
    auto ApiElementIterator<T>::operator->() const -> T*
    {
        return impl->current->second.get();
    }

    template<typename T>
    auto ApiElementIterator<T>::operator++() -> ApiElementIterator<T>&
    {
        impl->current++;
        return *this;
    }

    template<typename T>
    auto ApiElementIterator<T>::operator++(int) -> ApiElementIterator<T>
    {
        ApiElementIterator<T> temp = *this;
        ++(*this);

        return temp;
    }

    template<typename T>
    auto ApiElementIterator<T>::operator==(const ApiElementIterator<T>& compare) const -> bool
    {
        return compare.impl->current == impl->current;
    }

    template<typename T>
    auto ApiElementIterator<T>::operator!=(const ApiElementIterator<T>& compare) const -> bool
    {
        return compare.impl->current != impl->current;
    }

    template class ApiElementIterator<FlightInformationRegion::FlightInformationRegion>;
    template class ApiElementIterator<FlowMeasure::FlowMeasure>;
    template class ApiElementIterator<Event::Event>;
}// namespace ECFMP::Api
