#pragma once

namespace ECFMP::Api {
    /**
     * An element that comes from the API.
     */
    class ApiElement
    {
        public:
        /**
         * Returns the numerical id of the API element.
         */
        [[nodiscard]] virtual auto Id() const noexcept -> int = 0;
    };

    /**
     * An element that comes from the API that can be identified by a string value, for example
     * an FIR.
     */
    class StringIdentifiableApiElement : public ApiElement
    {
        public:
        /**
         * Returns the string identifier of the API element, e.g. "EGTT".
         */
        [[nodiscard]] virtual auto Identifier() const noexcept -> const std::string& = 0;
    };
}// namespace ECFMP::Api
