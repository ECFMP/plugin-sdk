#pragma once
#include "ECFMP/api/ApiElementCollection.h"

namespace ECFMP::Api {

    template<typename T>
    class StringIdentifierApiElementCollection : public ApiElementCollection<T>
    {
        public:
        [[nodiscard]] auto ContainsByIdentifier(const std::string& identifier) const -> bool
        {
            return this->Contains([&identifier](const auto& element) {
                return element.Identifier() == identifier;
            });
        }

        [[nodiscard]] auto FirstByIdentifier(const std::string& identifier) const -> std::shared_ptr<const T>
        {
            return this->First([&identifier](const auto& element) {
                return element.Identifier() == identifier;
            });
        }
    };
}// namespace ECFMP::Api
