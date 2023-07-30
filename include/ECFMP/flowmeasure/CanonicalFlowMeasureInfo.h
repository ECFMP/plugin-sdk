#pragma once

namespace ECFMP::FlowMeasure {

    /**
         * Contains information about a canonical flow measure.
         */
    class CanonicalFlowMeasureInfo
    {
        public:
        CanonicalFlowMeasureInfo(const std::string& identifier);
        [[nodiscard]] auto Identifier() const noexcept -> const std::string&;
        [[nodiscard]] auto Revision() const noexcept -> int;
        [[nodiscard]] auto IsAfter(const CanonicalFlowMeasureInfo& other) const noexcept -> bool;

        private:
        [[nodiscard]] static auto ParseIdentifier(const std::string& identifier) -> std::string;
        [[nodiscard]] static auto ParseRevision(const std::string& identifier) -> int;

        // The identifier of the canonical flow measure
        std::string identifier;

        // The revision of the canonical flow measure
        int revision;
    };

}// namespace ECFMP::FlowMeasure
