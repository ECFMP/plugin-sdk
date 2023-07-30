#include "ECFMP/flowmeasure/CanonicalFlowMeasureInfo.h"

namespace ECFMP::FlowMeasure {
    CanonicalFlowMeasureInfo::CanonicalFlowMeasureInfo(const std::string& identifier)
        : identifier(ParseIdentifier(identifier)), revision(ParseRevision(identifier))
    {}

    auto CanonicalFlowMeasureInfo::Identifier() const noexcept -> const std::string&
    {
        return identifier;
    }

    auto CanonicalFlowMeasureInfo::Revision() const noexcept -> int
    {
        return revision;
    }

    auto CanonicalFlowMeasureInfo::IsAfter(const CanonicalFlowMeasureInfo& other) const noexcept -> bool
    {
        return identifier == other.identifier && revision > other.revision;
    }

    auto CanonicalFlowMeasureInfo::ParseIdentifier(const std::string& identifier) -> std::string
    {
        return identifier.substr(0, identifier.find_last_of('-'));
    }

    auto CanonicalFlowMeasureInfo::ParseRevision(const std::string& identifier) -> int
    {
        auto revisionSeparator = identifier.find_last_of('-');
        if (revisionSeparator == std::string::npos) {
            return 0;
        }

        auto revision = identifier.substr(revisionSeparator + 1);
        try {
            return std::stoi(revision);
        }
        catch (std::invalid_argument&) {
            return 0;
        }
    }
}// namespace ECFMP::FlowMeasure
